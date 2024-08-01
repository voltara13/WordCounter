#pragma once

#include <QtQmlIntegration>

class WorkerThread;

class WordCounterController : public QObject
{
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QUrl filePath READ GetFilePath WRITE SetFilePath NOTIFY filePathChanged)
  Q_PROPERTY(QVariantList histogram READ GetHistogramList NOTIFY histogramChanged)
  Q_PROPERTY(bool isCanceled MEMBER _isCanceled NOTIFY isCanceledChanged)
  Q_PROPERTY(bool isPaused MEMBER _isPaused NOTIFY isPausedChanged)

signals:
  void filePathChanged();
  void isCanceledChanged();
  void isPausedChanged();
  void histogramChanged();

public:
  WordCounterController(QObject* ipParent = nullptr);
  ~WordCounterController();

  QVariantList GetHistogramList() const;
  QUrl GetFilePath() const;
  void SetFilePath(const QUrl& iNewFilePath);

public slots:
  void start();
  void pause();
  void abort();

private:
  void SetIsCanceled(bool iNewIsCanceled);
  void SetIsPaused(bool iNewIsPaused);
  void SetHistogram(QVariantMap iHistogram);

  QVariantMap _histogram;
  QPointer<WorkerThread> _workerThread;
  QUrl _filePath;

  bool _isCanceled = true;
  bool _isPaused = false;
};