#pragma once

#include <QtQmlIntegration>

class WorkerThread;

class WordCounterController : public QObject
{
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QUrl filePath READ GetFilePath WRITE SetFilePath NOTIFY filePathChanged)
  Q_PROPERTY(bool isCanceled MEMBER _isCanceled NOTIFY isCanceledChanged)
  Q_PROPERTY(bool isPaused MEMBER _isPaused NOTIFY isPausedChanged)
  Q_PROPERTY(qreal progress MEMBER _progress NOTIFY progressChanged)


signals:
  void filePathChanged();
  void isCanceledChanged();
  void isPausedChanged();
  void progressChanged();

public:
  WordCounterController(QObject* ipParent = nullptr);
  ~WordCounterController();

  QUrl GetFilePath() const;
  void SetFilePath(const QUrl& iNewFilePath);

public slots:
  void start();
  void pause();
  void abort();

private:
  void SetIsCanceled(bool iNewIsCanceled);
  void SetIsPaused(bool iNewIsPaused);
  void SetProgress(qreal iNewProgress);
  void SetWordsCount(const QMap<QString, int>& iWordsCount);

  QPointer<WorkerThread> _workerThread;
  QUrl _filePath;
  qreal _progress = 0.0;

  bool _isCanceled = true;
  bool _isPaused = false;
};