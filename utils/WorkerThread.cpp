#include "WorkerThread.h"

#include <QMutexLocker>
#include <QTextStream>
#include <QRegularExpression>
#include <QException>

WorkerThread::WorkerThread(const QString& iFilePath, QObject* ipParent) : QThread(ipParent)
{
  _file.setFileName(iFilePath);
  if (!_file.open(QIODevice::ReadOnly))
    throw std::runtime_error("Failed to open file: " + iFilePath.toStdString());
}

WorkerThread::~WorkerThread()
{
  if (_file.isOpen())
    _file.close();
}

void WorkerThread::SetIsPaused(bool iNewIsPaused)
{
  QMutexLocker locker(&_pauseMutex);
  if (_isPaused == iNewIsPaused)
    return;

  _isPaused = iNewIsPaused;

  if (!_isPaused)
    _pauseCondition.wakeAll();
}

void WorkerThread::run()
{
  try {
    QTextStream in(&_file);

    while (!isInterruptionRequested()) {
      QMap<QString, int> wordsCounts;

      while (!in.atEnd() && !isInterruptionRequested()) {
        const auto size = _file.size();
        const auto line = in.readLine();
        const auto words = line.split(QRegularExpression("\\W+"), Qt::SkipEmptyParts);

        for (const QString& word : words)
          wordsCounts[word.toLower()]++;

        if (size > 0)
          emit progressChanged(in.pos() * 100.0 / size);
        else
          emit progressChanged(0.0);

        QMutexLocker pauseLocker(&_pauseMutex);
        if (_isPaused)
          _pauseCondition.wait(&_pauseMutex);
      }

      _file.seek(0);

      emit wordsCountsReady(wordsCounts);
    }
  }
  catch (...) {}
}
