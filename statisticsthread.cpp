#include "statisticsthread.h"
#include <QDirIterator>
#include <QFile>
#include <QTextStream>

StatisticsThread::StatisticsThread(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<StatisticsInfo>("StatisticsInfo");

    moveToThread(&m_thread);
    connect(&m_thread, &QThread::started, this, &StatisticsThread::onThreadStarted);
    connect(&m_thread, &QThread::finished, this, &StatisticsThread::onThreadFinished);
    m_thread.start();
}

void StatisticsThread::stopThread()
{
    m_thread.quit();
    m_thread.wait();
}

void StatisticsThread::startStatistics(const QString &path)
{
    QMetaObject::invokeMethod(this, "onStartStatistics", Q_ARG(QString, path));
}

void StatisticsThread::onThreadStarted()
{

}

void StatisticsThread::onThreadFinished()
{

}

void StatisticsThread::onStartStatistics(const QString &path)
{
    QStringList nameFilters;
    nameFilters << QString("*.h");
    nameFilters << QString("*.cpp");

    m_info = StatisticsInfo();

    QDirIterator iter(path, nameFilters, QDir::Files, QDirIterator::Subdirectories);
    while (iter.hasNext())
    {
        const QString currentPath = iter.next();
        const QFileInfo &fileInfo = iter.fileInfo();

        m_info.path = fileInfo.absoluteFilePath();

        QString suffix = fileInfo.completeSuffix();
        if (suffix == QString("h"))
        {
            m_info.hFileCount++;

            QFile file(fileInfo.absoluteFilePath());
            if (file.open(QFile::ReadOnly))
            {
                QTextStream in(&file);
                QString line;
                while (!(line = in.readLine()).isNull())
                {
                    m_info.hLineCount++;
                    if (line.trimmed().isEmpty())
                    {
                        m_info.hBlankLineCount++;
                    }
                }
            }
            file.close();
        }
        else if (suffix == QString("cpp"))
        {
            m_info.cppFileCount++;

            QFile file(fileInfo.absoluteFilePath());
            if (file.open(QFile::ReadOnly))
            {
                QTextStream in(&file);
                QString line;
                while (!(line = in.readLine()).isNull())
                {
                    m_info.cppLineCount++;
                    if (line.trimmed().isEmpty())
                    {
                        m_info.cppBlankLineCount++;
                    }
                }
            }
            file.close();
        }
        //
        emit sig_statisticsInfo(m_info);
    }
    //
    emit sig_finished();
}
