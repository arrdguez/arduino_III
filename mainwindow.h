#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>



#include <QMainWindow>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>


//#include <QChart>
//#include <QChartView>
#include "QtCharts/QChartView"
#include "QtCharts/QtCharts"
#include "QtCharts/QLineSeries"
#include "QtCharts/QLegend"
#include "QtCharts/QValueAxis"
#include "QtCharts/QAbstractAxis"
#include "QtCharts/QXYSeries"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    //begin seriar comunication stuff
      QSerialPort *arduino = new QSerialPort(); // all the information related with the arduino will be in the object

      QString serial_port_name;
      bool on_off;
      bool arduino_is_avail;
      QStringList list;

    //end seriar comunication stuff

    //begin plot declarations
      QLineSeries *lineserie = new QLineSeries();
      QLineSeries *lineserie_b = new QLineSeries();

      QChart *chart = new QChart();
      QChartView *chartview = new QChartView();
      QValueAxis *axisX = new QValueAxis();
      QValueAxis *axis2X = new QValueAxis();
      QValueAxis *axisY = new QValueAxis();
      QValueAxis *axis2Y = new QValueAxis();
      int count;
    //end plot declarations




    int setup_plot();
    int plot();

private slots:

  void read_from_arduino();



    void on_setup_serial_port_clicked();

    void on_plot_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
