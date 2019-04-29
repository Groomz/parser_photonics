#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTime>
#include <QDebug>
#include <QTest>
#include <stdio.h>
#include <QFile>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QSerialPort *serial; // SerialPort
    int firstWaitTime = 500; // Первичное время ожидания новых данных, мс
    int additionalWaitTime = 10; // Дополнительное время ожидания новых данных, мс

public:
     explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QStringList SepList;
    QString SepWithOut;
    QString Remainder;
    QVector<double> DataVector;
    QVector<double> NumberVector;
    QVector<double> TimerVector;
    QTime timer;
    bool IsCycle = false;
    int count = 0;
    QFile pfile;

private slots:
    void on_action_triggered();
    void on_pushButton_Format_clicked();
    void on_pushButton_Data_clicked();
    void on_pushButton_Connect_clicked();
    void on_pushButton_Cycle_clicked();
    void on_pushButton_Clear_clicked();
    void on_pushButton_update_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
