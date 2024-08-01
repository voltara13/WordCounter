#include "WorkerThread.h"

#include <QMutexLocker>

WorkerThread::WorkerThread(QObject* ipParent) : QThread(ipParent)
{
}

void WorkerThread::SetIsPaused(bool iNewIsPaused)
{
  if (_isPaused == iNewIsPaused)
    return;

  QMutexLocker locker(&_mutex);
  _isPaused = iNewIsPaused;

  if (!_isPaused)
    _pauseCondition.wakeAll();
}

void WorkerThread::run()
{
  int i = 0;

  while (!isInterruptionRequested()) {
    QThread::msleep(10);

    i = (i + 1) % 100;
    emit progressChanged(i);

    QMutexLocker locker(&_mutex);
    if (_isPaused)       
      _pauseCondition.wait(&_mutex);
  }
}
