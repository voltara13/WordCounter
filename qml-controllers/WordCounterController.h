#pragma once

#include <QtQmlIntegration>

class WordCounterController : public QObject
{
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QUrl filePath READ GetFilePath WRITE SetFilePath NOTIFY filePathChanged)
  Q_PROPERTY()

signals:
  void filePathChanged();

public:
  WordCounterController(QObject* ipParent = nullptr);

  QUrl GetFilePath() const;

  void SetFilePath(const QUrl& iNewFilePath);

private:
  QUrl _filePath;

  bool _isCanceled = false;
  bool _isPaused = false;
};