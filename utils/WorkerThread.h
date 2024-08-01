#pragma once

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class WorkerThread: public QThread
{
  Q_OBJECT

signals:
  void progressChanged(int iProgress);

public:
  WorkerThread(QObject* ipParent = nullptr);

  void SetIsPaused(bool iNewIsPaused);

private:
  void run() override;

  QMutex _mutex;
  QWaitCondition _pauseCondition;

  bool _isPaused = false;
};