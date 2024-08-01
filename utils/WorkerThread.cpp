#include "WorkerThread.h"

#include <QMutexLocker>
#include <QTextStream>
#include <QRegularExpression>
#include <QException>
#include <QThreadPool>
#include <QFuture>
#include <QtConcurrent>

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
  QTextStream in(&_file);
  in.setEncoding(QStringConverter::Encoding::Utf8);
  const auto fileSize = _file.size();

  QThreadPool pool;
  pool.setMaxThreadCount(QThread::idealThreadCount());

  while (!isInterruptionRequested()) {
    QString leftover;
    QVariantMap wordsCounts;
    QList<QFuture<QVariantMap>> futures;

    while (!in.atEnd() && !isInterruptionRequested()) {
      auto block = in.read(qMin(fileSize, 1024 * 1024));
      block = leftover + block;
      leftover.clear();

      int lastValidPos = block.lastIndexOf(QRegularExpression("\\W"));
      if (lastValidPos != -1 && lastValidPos != block.size() - 1) {
        leftover = block.mid(lastValidPos + 1);
        block = block.left(lastValidPos + 1);
      }

      auto future = QtConcurrent::run(&pool, [block, this]() -> QVariantMap {
        if (isInterruptionRequested())
          return {};

        QMutexLocker pauseLocker(&_pauseMutex);
        if (_isPaused)
          _pauseCondition.wait(&_pauseMutex);

        QVariantMap localCounts;
        const auto lines = block.split(QRegularExpression("[\\r\\n]"), Qt::SkipEmptyParts);
        for (const auto& line : lines) {
          const auto words = line.split(QRegularExpression("\\W+"), Qt::SkipEmptyParts);
          for (const auto& word : words) {
            if (localCounts.contains(word.toLower()))
              localCounts[word.toLower()] = localCounts[word.toLower()].toInt() + 1;
            else
              localCounts[word.toLower()] = 1;
          }
        }

        return localCounts;
        });

      futures.append(future);
    }

    for (auto& future : futures) {
      future.waitForFinished();
      const auto result = future.result();
      for (auto it = result.cbegin(); it != result.cend(); ++it) {
        const auto& key = it.key();
        if (wordsCounts.contains(key))
          wordsCounts[key] = wordsCounts[key].toInt() + result[key].toInt();
        else
          wordsCounts[key] = result[key];
      }
    }

    if (!isInterruptionRequested())
      emit wordsCountsReady(wordsCounts);

    in.seek(0);
  }
}
