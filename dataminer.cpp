#include "dataminer.h"

#include <QVariant>
#include <QTextStream>

DataMiner::DataMiner(QObject *parent) : QObject(parent)
{

    currentHumidity = 0;
    currentTemperature = 10;

    if(connectToRS485() != -1)
    {
        startLumelResponce();
    }
    else
    {
        emit errorDetected("Unable to connect RS485");
        currentHumidity = -1;

    }
     //startLumelResponce();
}

int DataMiner::connectToRS485()
{
    modbusDevice = new QModbusRtuSerialMaster(this);
    if(!modbusDevice)   return -1;


    if (modbusDevice->state() != QModbusDevice::ConnectedState)
    {

        modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter, settings.port);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter, settings.parity);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, settings.baud);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, settings.dataBits);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, settings.stopBits);

        modbusDevice->setTimeout(settings.responseTime);
        modbusDevice->setNumberOfRetries(settings.numberOfRetries);

        if(!modbusDevice->connectDevice())
        {
            QTextStream err(stderr);
            err << tr("Connect failed: ") + modbusDevice->errorString();
            emit errorDetected(tr("Connect failed: ") + modbusDevice->errorString());
            return -1;
            //currentTemperature=-1;
        }
    }

    return 1;
}

bool DataMiner::startLumelResponce()
{
    if(!modbusDevice)
        return false;
    if(!lumel)
        lumel = new LumelP18(modbusDevice, 3, this);

    connect(lumel, &LumelP18::updateInputs, this, &DataMiner::updateTH);
    connect(lumel, &LumelP18::errorDetected, this, &DataMiner::errDetect);

    lumel->start(1000);
//currentTemperature = -2;
    return true;
}

void DataMiner::updateTH()
{
    currentHumidity = lumel->getHumidity();
    currentTemperature = lumel->getTemperature();
}

float DataMiner::getCurrentTemperature()
{
    return currentTemperature;
}

float DataMiner::getCurrentHumidity()
{
    return currentHumidity;
}

void DataMiner::errDetect(QString errMsg)
{
    emit errorDetected(errMsg);

}
