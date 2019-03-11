#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
}

MainWindow::~MainWindow()
{
    if (m_statisticsThread)
    {
        m_statisticsThread->stopThread();
        delete m_statisticsThread;
    }

    delete ui;
}

void MainWindow::onTimer()
{
    m_seconds++;
    ui->label_time->setText(QString("耗时：%1秒").arg(m_seconds));
}

void MainWindow::onStatisticsInfo(StatisticsInfo info)
{
    ui->lineEdit_hFileCount->setText(QString::number(info.hFileCount));
    ui->lineEdit_hLineCount->setText(QString::number(info.hLineCount));
    ui->lineEdit_hBlankLineCount->setText(QString::number(info.hBlankLineCount));
    ui->lineEdit_cppFileCount->setText(QString::number(info.cppFileCount));
    ui->lineEdit_cppLineCount->setText(QString::number(info.cppLineCount));
    ui->lineEdit_cppBlankLineCount->setText(QString::number(info.cppBlankLineCount));

    ui->label_progress->setText(QString("进度：%1").arg(info.path));

    ui->lineEdit_allFileCount->setText(QString::number(info.cppFileCount + info.hFileCount));
    ui->lineEdit_allLineCount->setText(QString::number(info.cppLineCount + info.hLineCount));
    ui->lineEdit_allBlankLineCount->setText(QString::number(info.cppBlankLineCount + info.hBlankLineCount));
}

void MainWindow::onStatisticsFinished()
{
    m_timer->stop();
    ui->label_time->setText(QString("耗时：%1秒").arg(m_seconds));

    ui->label_progress->setText(QString("进度：完成"));
    ui->pushButton_begin->setEnabled(true);
}

void MainWindow::on_pushButton_open_clicked()
{
    QString strPath = QFileDialog::getExistingDirectory(this, "选择目录");
    if (strPath.isEmpty())
    {
        return;
    }

    ui->lineEdit_path->setText(strPath);

    ui->lineEdit_hFileCount->clear();
    ui->lineEdit_hLineCount->clear();
    ui->lineEdit_hBlankLineCount->clear();
    ui->lineEdit_cppFileCount->clear();
    ui->lineEdit_cppLineCount->clear();
    ui->lineEdit_cppBlankLineCount->clear();

    ui->label_progress->setText(QString("进度："));

    ui->lineEdit_allFileCount->clear();
    ui->lineEdit_allLineCount->clear();
    ui->lineEdit_allBlankLineCount->clear();
}

void MainWindow::on_pushButton_begin_clicked()
{
    const QString &strPath = ui->lineEdit_path->text();
    if (strPath.isEmpty())
    {
        QMessageBox::information(this, "提示", "请先选择目录！");
        return;
    }

    if (!m_statisticsThread)
    {
        m_statisticsThread = new StatisticsThread();
        connect(m_statisticsThread, &StatisticsThread::sig_statisticsInfo, this, &MainWindow::onStatisticsInfo);
        connect(m_statisticsThread, &StatisticsThread::sig_finished, this, &MainWindow::onStatisticsFinished);
    }
    m_statisticsThread->startStatistics(strPath);

    m_seconds = 0;
    m_timer->start(1000);
    ui->pushButton_begin->setEnabled(false);
}
