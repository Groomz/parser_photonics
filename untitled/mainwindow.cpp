#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <QTest>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_Format->setDisabled(true);
    ui->pushButton_Data->setDisabled(true);
    ui->lineEdit_Format->setDisabled(true);
    ui->lineEdit_Data->setDisabled(true);
    ui->lineEdit_separator->setEnabled(false);
    ui->pushButton_Cycle->setDisabled(true);

    QString temp;

    QStringList options;

    options.push_back("600");
    options.push_back("1200");
    options.push_back("2400");
    options.push_back("4800");
    options.push_back("9600");
    options.push_back("19200");
    options.push_back("38400");
    options.push_back("57600");
    options.push_back("115200");
    ui->comboBox_BaudRate->addItems(options);
    options.clear();
    ui->comboBox_BaudRate->setCurrentText("9600");

    options.push_back("No Parity");
    options.push_back("Even Parity");
    options.push_back("Odd Parity");
    options.push_back("Space Parity");
    options.push_back("Mark Parity");
    ui->comboBox_ParityControl->addItems(options);
    options.clear();
    ui->comboBox_ParityControl->setCurrentText("No Parity");

    options.push_back("1");
    options.push_back("1.5");
    options.push_back("2");
    ui->comboBox_StopBit->addItems(options);
    options.clear();
    ui->comboBox_StopBit->setCurrentText("1");

    options.push_back("5");
    options.push_back("6");
    options.push_back("7");
    options.push_back("8");
    ui->comboBox_DataBits->addItems(options);
    options.clear();
    ui->comboBox_DataBits->setCurrentText("8");

    options.push_back("No Control");
    options.push_back("Hardware Control");
    options.push_back("Software Control");
    ui->comboBox_FlowControl->addItems(options);
    options.clear();
    ui->comboBox_FlowControl->setCurrentText("No Control");


    on_action_triggered();


    serial = new QSerialPort();  // Создать новый объект класса "SerialPort"

    connect(this, SIGNAL(response(QString)),
            this, SLOT(get_response(QString))); // Подключить сигнал получения ответа к слоту



    ui->lineEdit_separator->setText("d");
    ui->lineEdit_Format->setText("d:d:d:d$");
    ui->lineEdit_Data->setText("FName of file with data");

//    on_pushButton_Format_clicked();
//    on_pushButton_Data_clicked();

    ui->CustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_triggered()
{
    ui->comboBox_PortName->clear();

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBox_PortName->addItem(info.portName());
    }

   // ui->lineEdit_Response->setText("Clicked!");

    return;
}




void MainWindow::on_pushButton_Format_clicked()
{
    QString separator = ui->lineEdit_separator->text();
    QString Format = ui->lineEdit_Format->text();
    SepList = Format.split(separator, QString::SkipEmptyParts, Qt::CaseInsensitive);
    SepWithOut = SepList[SepList.size()-1];

    ui->lineEdit_separator->setEnabled(false);
    ui->lineEdit_Format->setDisabled(true);
    ui->pushButton_Format->setDisabled(true);
    ui->pushButton_Data->setDisabled(false);
    ui->lineEdit_Data->setDisabled(true);
    ui->pushButton_Cycle->setDisabled(false);

//    SepList.removeLast();

    QString start = "1";
    serial->write(start.toLocal8Bit());
    timer.start();


    pfile.setFileName(ui->lineEdit_Data->text());
    pfile.open(QIODevice::Append  | QIODevice::Text);
    QString s = "Data Format    " + ui->lineEdit_Format->text() + "\n";
    QTextStream stream(&pfile);
    stream << s;
    pfile.close();
}

void MainWindow::on_pushButton_Data_clicked()
{
    ui->textEdit_Result->clear();

    QString recived;
    QByteArray array;

    this->firstWaitTime = 5000;

    if (serial->waitForReadyRead(this->firstWaitTime)) { // Если за данное число миллисекунд что-то пришло
        array = serial->readAll(); // Прочитать полученные данные
        while (serial->waitForReadyRead(this->additionalWaitTime)) // Если пришло что-то ещё
            array += serial->readAll(); // Дописать новые данные
        recived = QString(array); // Преобразовать полученные данные в строку

       // ui->lineEditTTR->setText(QString::number(time.elapsed()-this->additionalWaitTime)); // Записать время отклика
    }

   // QString DataStr = Remainder + ui->lineEdit_Data->text();
    QString DataStr = Remainder + recived;
    Remainder.clear();

    QStringList DataListFirst = DataStr.split(SepWithOut, QString::SkipEmptyParts, Qt::CaseInsensitive);

    if(!DataStr.endsWith(SepWithOut, Qt::CaseInsensitive))
    {
         Remainder =  DataListFirst.last();
         DataListFirst.removeLast();
    }

   // int count = 0;

    int ColourStep = 256/SepList.size()-1;

    for (int i = 0; i < SepList.size(); i++)
    {
         ui->CustomPlot->addGraph();
         ui->CustomPlot->graph(i)->setPen(QPen(QColor(0 + ColourStep*i, 110, 255 - ColourStep*i)));
    }

    pfile.open(QIODevice::Append | QIODevice::Text);
    QString s;
    QTextStream stream(&pfile);


    for (int i = 0; i < DataListFirst.size(); i++)
    {

        s += DataListFirst[i] + "     " + QString::number(timer.elapsed()/1000.0) + "\n";


        for (int j = 0; j < SepList.size(); j++)
        {
           QStringList DataListSecond = DataListFirst[i].split(SepList[j], QString::SkipEmptyParts, Qt::CaseInsensitive);

           ui->textEdit_Result->append(DataListSecond[0]);

           double temp = DataListSecond[0].toDouble();

           ++count;

           DataVector.append(temp);
           NumberVector.append(count);
           TimerVector.append(timer.elapsed()/1000.0);

           DataListSecond.removeFirst();

           DataListFirst[i] = DataListSecond.join(SepList[j]);

        }
    }

    stream << s;
    pfile.close();


  //  QVector<QVector<double>> Graphs;

    for(int i = 0; i < SepList.size(); i++)
    {
        QVector<double> V_temp;
        QVector<double> N_temp;
        QVector<double> T_temp;
        for (int k = 0; k < DataVector.size(); k = k + SepList.size())
        {
            V_temp.append(DataVector[k+i]);
            N_temp.append(k+i);
            T_temp.append(TimerVector[k+i]);
        }
      //  Graphs.append(V_temp);
        ui->CustomPlot->graph(i)->setData(T_temp, V_temp);
    }

    ui->CustomPlot->xAxis->setLabel("t");
    ui->CustomPlot->yAxis->setLabel("Data");
    ui->CustomPlot->rescaleAxes();
    ui->CustomPlot->replot();
}



void MainWindow::on_pushButton_Connect_clicked()
{
    ui->lineEdit_separator->setEnabled(true);
    ui->lineEdit_Format->setDisabled(false);
    ui->pushButton_Format->setDisabled(false);
    ui->lineEdit_Data->setDisabled(false);
    ui->pushButton_update->setDisabled(!false);



    if (ui->pushButton_Connect->text() == "Connect")
    {

//здесь еще не падал

        serial->setPortName(ui->comboBox_PortName->currentText());

//здесь упал

//        if(ui->comboBox_BaudRate->currentText() == "600")
//            serial->setBaudRate(QSerialPort::Baud600);
        if(ui->comboBox_BaudRate->currentText() == "1200")
            serial->setBaudRate(QSerialPort::Baud1200);
        if(ui->comboBox_BaudRate->currentText() == "2400")
            serial->setBaudRate(QSerialPort::Baud2400);
        if(ui->comboBox_BaudRate->currentText() == "4800")
            serial->setBaudRate(QSerialPort::Baud4800);
        if(ui->comboBox_BaudRate->currentText() == "9600")
            serial->setBaudRate(QSerialPort::Baud9600);
        if(ui->comboBox_BaudRate->currentText() == "19200")
            serial->setBaudRate(QSerialPort::Baud19200);
        if(ui->comboBox_BaudRate->currentText() == "38400")
            serial->setBaudRate(QSerialPort::Baud38400);
        if(ui->comboBox_BaudRate->currentText() == "57600")
            serial->setBaudRate(QSerialPort::Baud57600);
        if(ui->comboBox_BaudRate->currentText() == "115200")
            serial->setBaudRate(QSerialPort::Baud115200);



        if(ui->comboBox_FlowControl->currentText() == "No")
            serial->setFlowControl(QSerialPort::NoFlowControl);
        if(ui->comboBox_FlowControl->currentText() == "Software")
            serial->setFlowControl(QSerialPort::SoftwareControl);
        if(ui->comboBox_FlowControl->currentText() == "Hardware")
            serial->setFlowControl(QSerialPort::HardwareControl);


        if(ui->comboBox_DataBits->currentText() == "5")
            serial->setDataBits(QSerialPort::Data5);
        if(ui->comboBox_DataBits->currentText() == "6")
            serial->setDataBits(QSerialPort::Data6);
        if(ui->comboBox_DataBits->currentText() == "7")
            serial->setDataBits(QSerialPort::Data7);
        if(ui->comboBox_DataBits->currentText() == "8")
            serial->setDataBits(QSerialPort::Data8);

        if(ui->comboBox_ParityControl->currentText() == "No Parity")
            serial->setParity(QSerialPort::NoParity);
        if(ui->comboBox_ParityControl->currentText() == "Even")
            serial->setParity(QSerialPort::EvenParity);
        if(ui->comboBox_ParityControl->currentText() == "Odd")
            serial->setParity(QSerialPort::OddParity);
        if(ui->comboBox_ParityControl->currentText() == "Mark")
            serial->setParity(QSerialPort::MarkParity);
        if(ui->comboBox_ParityControl->currentText() == "Space")
            serial->setParity(QSerialPort::SpaceParity);

        if(ui->comboBox_StopBit->currentText() == "1")
            serial->setStopBits(QSerialPort::OneStop);
        if(ui->comboBox_StopBit->currentText() == "1.5")
            serial->setStopBits(QSerialPort::OneAndHalfStop);
        if(ui->comboBox_StopBit->currentText() == "2")
            serial->setStopBits(QSerialPort::TwoStop);

// сюда уже не дошел


        if (!serial->open(QIODevice::ReadWrite))
        {
            QSerialPort::SerialPortError getError = QSerialPort::NoError;
            serial->error(getError);

    //        emit response(tr("Can't open %1, error code %2").arg(ui->comboBox_PortName->currentText()).arg(serial.error()));

            return;
        }

        ui->pushButton_Connect->setText("Disconnect");
    } else
        {
            serial->close();

            ui->pushButton_Connect->setText("Connect");

            IsCycle = false;

            ui->lineEdit_separator->setEnabled(!true);
            ui->lineEdit_Format->setDisabled(!false);
            ui->pushButton_Format->setDisabled(!false);
            ui->lineEdit_Data->setDisabled(!false);
            ui->pushButton_update->setDisabled(false);
        }
}


void MainWindow::on_pushButton_Cycle_clicked()
{
    if(IsCycle == false)
    {
        IsCycle = true;
        ui->pushButton_Cycle->setText("Stop");
    }
    else
    {
        IsCycle = false;
        ui->pushButton_Cycle->setText("Start");
    }

    while(IsCycle == true)
    {
        on_pushButton_Data_clicked();
        QTest::qWait(15);

    }
    ui->pushButton_Cycle->setText("Start");
}

void MainWindow::on_pushButton_Clear_clicked()
{
    DataVector.clear();
    NumberVector.clear();
    TimerVector.clear();
    ui->CustomPlot->clearGraphs();
}

void MainWindow::on_pushButton_update_clicked()
{
    ui->comboBox_PortName->clear();

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBox_PortName->addItem(info.portName());
    }

   // ui->lineEdit_Response->setText("Clicked!");

    return;
}
