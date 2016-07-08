#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "plotter.h"
#include "dataminer.h"
#include <QTimer>
#include <QModbusRtuSerialMaster>
#include <QSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int setTemperature(float t);

    int setHumidity(float humi);

    QModbusRtuSerialMaster *modbusDevice;

private slots:
    void on_pbTemperatureUstavka_clicked();

    void on_pbHumiUstavka_clicked();

    void on_pbPlotSettings_clicked();

    void on_pbStart_clicked();

    void addDataToPlot();

    void errDetect(QString msg);

    void on_pbForwardForward_clicked();

    void on_pbForward_clicked();

    void on_pbBack_clicked();

    void on_pbBackBack_clicked();

    void on_pbCurrent_clicked();

private:
    Ui::MainWindow *ui;

    float temperature_u=0;
    float humidity_u=0;

    DataMiner *dataminer;
    QTimer *timer;

    bool plotStarted;
};

#endif // MAINWINDOW_H
