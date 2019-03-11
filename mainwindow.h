#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "statisticsthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTimer();

    void onStatisticsInfo(StatisticsInfo info);
    void onStatisticsFinished();

    void on_pushButton_open_clicked();
    void on_pushButton_begin_clicked();

private:
    Ui::MainWindow *ui;

    QTimer *m_timer;
    int m_seconds = 0;

    StatisticsThread *m_statisticsThread = nullptr;
};

#endif // MAINWINDOW_H
