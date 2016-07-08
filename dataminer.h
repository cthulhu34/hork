#ifndef DATAMINER_H
#define DATAMINER_H

#include <QObject>
#include <QModbusRtuSerialMaster>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QVector>
#include <QTimer>

#include "lumelp18.h"

class DataMiner : public QObject
{
    Q_OBJECT
public:
    explicit DataMiner(QObject *parent = 0);

private:
    struct Settings {
        QString port = "ttyUSB0";
        int parity = QSerialPort::NoParity;
        int baud = QSerialPort::Baud57600;
        int dataBits = QSerialPort::Data8;
        int stopBits = QSerialPort::OneStop;
        int responseTime = 1000;
        int numberOfRetries = 3;
    };
    QModbusRtuSerialMaster* modbusDevice;
    Settings settings;
    LumelP18 *lumel = Q_NULLPTR;
    float currentTemperature, currentHumidity;

    int connectToRS485();
    bool startLumelResponce();


signals:
    void errorDetected(QString msg);

public slots:
    float getCurrentTemperature();
    float getCurrentHumidity();

private slots:
    void updateTH();
    void errDetect(QString errMsg);
};

#endif // DATAMINER_H
