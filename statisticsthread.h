#ifndef STATISTICSTHREAD_H
#define STATISTICSTHREAD_H

#include <QObject>
#include <QThread>

struct StatisticsInfo
{
    QString path;

    int hFileCount = 0;
    int hLineCount = 0;
    int hBlankLineCount = 0;

    int cppFileCount = 0;
    int cppLineCount = 0;
    int cppBlankLineCount = 0;
};
Q_DECLARE_METATYPE(StatisticsInfo)

class StatisticsThread : public QObject
{
    Q_OBJECT
public:
    explicit StatisticsThread(QObject *parent = nullptr);

    void stopThread();

    void startStatistics(const QString &path);

signals:
    void sig_statisticsInfo(StatisticsInfo info);
    void sig_finished();

private slots:
    void onThreadStarted();
    void onThreadFinished();

    void onStartStatistics(const QString &path);

private:
    QThread m_thread;

    StatisticsInfo m_info;
};

#endif // STATISTICSTHREAD_H
