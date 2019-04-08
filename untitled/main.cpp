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

//    QString temp;
//    QString format;

   // QString temp = "kukareku";
 //   QString sep = "and";

    //poka net user-interfaca mozhno schitat chto sroku poluchili po zaprosu na schitanie

   // QString str_data = temp + "qwa.qu..and crya. and a ya. to.matAnd kukareku and";


//    QStringList list1 = str.split("AND", QString::SkipEmptyParts);
//    QStringList list2 = str.split(".");
//    QStringList list3 = str.split("and", QString::SkipEmptyParts, Qt::CaseInsensitive);

//    QStringList DataList = str.split(sep, QString::SkipEmptyParts, Qt::CaseInsensitive);

//    if (! str.endsWith(sep, Qt::CaseInsensitive))
//    {
//        temp = DataList[DataList.size() - 1];
//        DataList.removeAt(DataList.size() - 1);
//    }

//    {
//        QString s;
//        for (int i = 0; i < DataList.size(); i++)
//            s += DataList[i] + "|";

//        QLabel *label = new QLabel(s);
//        label->show();
//    }



//    QString second_sep = ".";

//    for (int i = 0; i <DataList.size(); i++)
//    {
//     QStringList WithinDataList = DataList[i].split(second_sep, QString::SkipEmptyParts, Qt::CaseInsensitive);


//        QString s;
//        for (int k = 0; k < WithinDataList.size(); k++)
//           s += WithinDataList[k] + "$";

//        QLabel *label = new QLabel(s);
//        label->show();


//    }



    return a.exec();
}
