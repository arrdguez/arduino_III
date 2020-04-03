#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    count = 0;
      arduino_is_avail = 0;
      //QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(read_from_arduino()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_setup_serial_port_clicked()
{
  QSerialPortInfo serialportinfo;
  qDebug() << "Number of avilable ports:" << QSerialPortInfo::availablePorts().length();
  ui->plainTextEdit->insertPlainText("Available ports:"+QString::number(QSerialPortInfo::availablePorts().length()));


  foreach (const QSerialPortInfo serial , QSerialPortInfo::availablePorts()) {
    qDebug() << "Has vendor:"<< serial.hasVendorIdentifier();
    if (serial.hasVendorIdentifier())
      qDebug() << "Vendor ID:"<< serial.vendorIdentifier();
    if (serial.hasProductIdentifier())
      qDebug() << "Product Identifier:"<< serial.productIdentifier();
      qDebug() << "Busy:"<< serial.isBusy();
    if (!serial.isBusy()){
      arduino_is_avail = 1;
      serial_port_name = serial.portName();
    }
  }

  if (arduino_is_avail){
    arduino->setPortName(serial_port_name);
    qDebug() << serial_port_name;
    arduino->open(QSerialPort::ReadWrite);
    //arduino->open(QIODevice::ReadOnly);


    qDebug() << "Buffer size:" << arduino->readBufferSize();
    arduino->setBaudRate(QSerialPort::Baud9600);
    arduino->setDataBits(QSerialPort::Data8);
    arduino->setParity(QSerialPort::NoParity);
    arduino->setStopBits(QSerialPort::OneStop);
    arduino->setFlowControl(QSerialPort::NoFlowControl);
    qDebug() <<"The Arduino is ready Read and Write.";
    ui->plainTextEdit->setPlainText("The Arduino is ready to Read and Write.");
    //setup_plot();
  }
  else{
    qDebug() <<"NO se pudo abrir.";
    ui->plainTextEdit->setPlainText("NO se pudo abrir.");
  }
}

void MainWindow::read_from_arduino()
{
  QByteArray serialData = arduino->readAll();
  QString line = QString::fromStdString(serialData.toStdString());
  list = line.split(QRegExp("\\s+"));



  float value_a = list.at(0).toFloat();
  float value_b = list.at(1).toFloat();
  qDebug() <<value_a<<value_b;
  ui->plainTextEdit->insertPlainText(QString::number(count)+" "+QString::number(list.at(0).toFloat())+" "+QString::number(list.at(1).toFloat())+"\n");

  ui->progressBar->setValue(list.at(0).toFloat());
  ui->lcdNumber->display(list.at(0).toFloat());


  //lineserie->append(count, str.toFloat());
  //lineserie_b->append(count, value_b);


  //plot();
  count += 1;
}
