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

QVariantList WordCounterController::GetHistogramList() const
{
  QVariantList list;

  for (auto it = _histogram.begin(); it != _histogram.end(); ++it) {
    QVariantMap entry;
    entry["key"] = it.key();
    entry["value"] = it.value();
    list.append(entry);
  }

  std::sort(list.begin(), list.end(), [](const QVariant& iFirst, const QVariant& iSecond) {
    return iFirst.toMap()["value"].toInt() > iSecond.toMap()["value"].toInt();
  });

  return list;
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

      SetHistogram({});
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
      connect(_workerThread, &WorkerThread::wordsCountsReady, this, &WordCounterController::SetHistogram);
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

void WordCounterController::SetHistogram(QVariantMap iHistogram)
{
  if (_histogram == iHistogram)
    return;

  _histogram = iHistogram;

  emit histogramChanged();
}
