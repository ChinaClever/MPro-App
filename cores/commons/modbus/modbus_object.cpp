/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "modbus_object.h"

Modbus_Object::Modbus_Object(QObject *parent)
    : QObject{parent}
{
    m_type = QModbusDataUnit::HoldingRegisters;
}


void Modbus_Object::initModbusSerial(const QString &portName, qint32 baudRate)
{
    mSet.baud = baudRate;
    mSet.portName = portName;
    QModbusDevice *dev = modbusDevice();
    dev->setConnectionParameter(QModbusDevice::SerialPortNameParameter, mSet.portName);
    dev->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, mSet.baud);
    dev->setConnectionParameter(QModbusDevice::SerialParityParameter, mSet.parity);
    dev->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, mSet.dataBits);
    dev->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, mSet.stopBits);
}

void Modbus_Object::initModbusNet(const QString &address, int port)
{
    QModbusDevice *dev = modbusDevice();
    dev->setConnectionParameter(QModbusDevice::NetworkPortParameter,port);
    dev->setConnectionParameter(QModbusDevice::NetworkAddressParameter, address);
}

void Modbus_Object::throwError(const QString &err)
{
    qDebug() << err << modbusDevice()->errorString();
}

void Modbus_Object::disconnectModbus()
{
    QModbusDevice *dev = modbusDevice();
    if(dev)dev->disconnectDevice();
}

void Modbus_Object::onStateChanged(int state)
{
    QString str;
    switch (state) {
    case QModbusDevice::UnconnectedState: str = tr("Modbus 已断开"); break;
    case QModbusDevice::ConnectingState: str = tr("Modbus 连接中"); break;
    case QModbusDevice::ConnectedState: str = tr("Modbus 已连接"); break;
    case QModbusDevice::ClosingState: str = tr("Modbus 已关闭"); break;
    } qDebug() << str;
}

void Modbus_Object::handleDeviceError(QModbusDevice::Error newError)
{
    if(newError != QModbusDevice::NoError)
        throwError("Error Modbus");
}

bool Modbus_Object::connectDevice()
{
    QModbusDevice *dev = modbusDevice();
    if(dev->state() == QModbusDevice::ConnectedState) disconnectModbus();

    bool ret = dev->connectDevice();
    if(ret) ret = waitForState(QModbusDevice::ConnectedState);
    return ret;
}

bool Modbus_Object::isConnectedModbus()
{
    bool ret = false; QModbusDevice *dev = modbusDevice();
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    if(dev) if(dev->state() == QModbusDevice::ConnectedState) ret = true;
     //qDebug() << "Error: Modbus Device is not connected!";
    return ret;
}

bool Modbus_Object::waitForState(int state)
{
    bool ret = false;
    QModbusDevice *dev = modbusDevice();
    for(int i=0; i<mSet.responseTime; i+=10) {
        cm::mdelay(10); if(dev->state() == state) {ret = true; break;}
    } if(!ret) throwError("Wait For State");
    return ret;
}

bool Modbus_Object::replyFinished(QModbusReply *reply)
{
    bool ret = false;
    for(int i=0; i<mSet.responseTime; i+=10) {
        if (reply->error() == QModbusDevice::NoError) {
            ret = reply->isFinished(); if(ret) break; else cm::mdelay(10);
        } else {
            QModbusPdu::ExceptionCode code = reply->rawResult().exceptionCode();
            qDebug() << tr("modbus response error:%1 (modbus exception:0x%2)")
                        .arg(reply->errorString()).arg(code, -1, 16); break;
        }
    }
    return ret;
}
