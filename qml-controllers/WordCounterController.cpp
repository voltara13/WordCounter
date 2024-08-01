#include "WordCounterController.h"

#include "utils/WorkerThread.h"

#include <QMessageBox>

WordCounterController::WordCounterController(QObject* ipParent) : QObject(ipParent)
{
}

WordCounterController::~WordCounterController()
{
  abort();
}

QUrl WordCounterController::GetFilePath() const
{
  return _filePath;
}

void WordCounterController::SetFilePath(const QUrl& iNewFilePath)
{
  if (_filePath == iNewFilePath)
    return;

  _filePath = iNewFilePath;

  abort();

  emit filePathChanged();
}

void WordCounterController::start()
{
  SetIsCanceled(false);
  SetIsPaused(false);
}

void WordCounterController::pause()
{
  SetIsPaused(true);
}

void WordCounterController::abort()
{
  SetIsPaused(false);
  SetIsCanceled(true);
}

void WordCounterController::SetIsCanceled(bool iNewIsCanceled)
{
  if (_isCanceled == iNewIsCanceled)
    return;

  if (iNewIsCanceled) {
    if (_workerThread && _workerThread->isRunning()) {
      _workerThread->requestInterruption();
      _workerThread->wait();
    }
  }
  else {
    if (!(_workerThread && _workerThread->isRunning())) {
      try {
        _workerThread = new WorkerThread(_filePath.toLocalFile(), this);
      }
      catch (const std::runtime_error& e) {
        QMessageBox::warning(nullptr, QObject::tr("Open file error"), e.what());
        return;
      }
      connect(_workerThread, &WorkerThread::finished, _workerThread, &QObject::deleteLater);
      connect(_workerThread, &WorkerThread::progressChanged, this, &WordCounterController::SetProgress);
      connect(_workerThread, &WorkerThread::wordsCountsReady, this, &WordCounterController::SetWordsCount);
      _workerThread->start();
    }
  }

  _isCanceled = iNewIsCanceled;

  emit isCanceledChanged();
}

void WordCounterController::SetIsPaused(bool iNewIsPaused)
{
  if (!(_workerThread && _workerThread->isRunning()))
    return;

  if (_isPaused == iNewIsPaused)
    return;

  _isPaused = iNewIsPaused;

  _workerThread->SetIsPaused(_isPaused);

  emit isPausedChanged();
}

void WordCounterController::SetProgress(qreal iNewProgress)
{
  _progress = iNewProgress;

  emit progressChanged();
}

void WordCounterController::SetWordsCount(const QMap<QString, int>& iWordsCount)
{
  qDebug() << iWordsCount;
}
