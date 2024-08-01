#include "WordCounterController.h"

#include "utils/WorkerThread.h"

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

  _isCanceled = iNewIsCanceled;

  if (_isCanceled) {
    if (_workerThread && _workerThread->isRunning()) {
      _workerThread->requestInterruption();
      _workerThread->wait();
    }
  }
  else {
    if (!(_workerThread && _workerThread->isRunning())) {
      _workerThread = new WorkerThread(this);
      connect(_workerThread, &WorkerThread::finished, _workerThread, &QObject::deleteLater);
      connect(_workerThread, &WorkerThread::progressChanged, this, &WordCounterController::SetProgress);
      _workerThread->start();
    }
  }

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
  if (qFuzzyCompare(_progress, iNewProgress))
    return;

  _progress = iNewProgress;

  emit progressChanged();
}
