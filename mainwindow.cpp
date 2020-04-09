#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  count = 0;
  arduino_is_avail = 0;
  QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(read_from_arduino()));
  setup_plot();
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


  lineserie->append(count, value_a);

  chart->removeSeries(lineserie);
  //chart->removeAxis(axisX);
  //chart->removeAxis(axisY);

  axisX->setRange(0, count);
  axisY->setRange(10,34);
  chart->addSeries(lineserie);

  //axisX->setRange(count-5, count);

  //chart->addAxis(axisX, Qt::AlignBottom);
  //chart->addAxis(axisY, Qt::AlignLeft);

  lineserie->attachAxis(axisX);
  lineserie->attachAxis(axisY);



  //chart->createDefaultAxes();

  //lineserie->attachAxis(axisX);
  //lineserie->attachAxis(axisY);

  //lineserie_b->append(count, value_b);


  plot();
  count += 1;
}

int MainWindow::setup_plot()
{

  // Customize series
  QPen pen(QRgb(0xfdb157)); //select the color
  //QPen pen_b(QRgb(0x0b157f)); //select the color
  //pen.setWidth(1);
  lineserie->setPen(pen);  //paint the serie with the color of the pen
  //lineserie_b->setPen(pen_b);

  /*
  lineserie->append(1,2);
  lineserie->append(2,4);
  lineserie->append(3,8);
  lineserie->append(4,16);
  lineserie->append(5,32);
  */

  chart->addSeries(lineserie);
  //chart->addSeries(lineserie_b);

  /*Con las siguientes lineas puedo controlar los ejes  */
  axisX->setGridLineVisible(false);
  axisY->setGridLineVisible(false);

  chart->addAxis(axisX, Qt::AlignBottom);
  chart->addAxis(axis2X, Qt::AlignTop);
  chart->addAxis(axisY, Qt::AlignLeft);
  chart->addAxis(axis2Y, Qt::AlignRight);

  lineserie->attachAxis(axisX);
  lineserie->attachAxis(axisY);


  //lineserie_b->attachAxis(axisX);
  //lineserie_b->attachAxis(axisY);

  //axisX->setRange(0, 10);
  axisX->setTitleText("Data");
  //axisY->setRange(15, 35);
  axisY->setTitleText("Bytes");


  // Customize chart title
  QFont font;
  font.setPixelSize(18);
  chart->setTitleFont(font);
  chart->setTitleBrush(QBrush(Qt::black));
  chart->setTitle("Temperature");

  //chartview->setChart(chart);
  //ui->gridLayout->addWidget(chartview);
  //ui->tabWidget->insertTab(0,chartview,"Plot");

  return 0;
}

int MainWindow::plot()
{
  chart->removeSeries(lineserie);
  chart->addSeries(lineserie);

  chartview->setChart(chart);
  //ui->gridLayout->removeWidget(chartview);
  ui->gridLayout_2->addWidget(chartview);
  return 0;
}

void MainWindow::on_plot_clicked()
{
    plot();
}
