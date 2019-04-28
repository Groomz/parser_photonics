#include "mainwindow.h"
#include <QApplication>
#include <QString>
#include <QLabel>
#include <QList>
#include <QStringList>
#include <iostream>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
