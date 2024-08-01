#include "WordCounterController.h"

WordCounterController::WordCounterController(QObject* ipParent) : QObject(ipParent)
{
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
