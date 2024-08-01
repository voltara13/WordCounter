#pragma once

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QFile>

class WorkerThread: public QThread
{
  Q_OBJECT

signals:
  void wordsCountsReady(QVariantMap iWordsMap);

public:
  WorkerThread(const QString& iFilePath = "", QObject* ipParent = nullptr);
  ~WorkerThread();

  void SetIsPaused(bool iNewIsPaused);

private:
  void run() override;

  QMutex _pauseMutex;
  QWaitCondition _pauseCondition;
  QFile _file = {};

  bool _isPaused = false;
};