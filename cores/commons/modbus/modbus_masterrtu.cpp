/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "modbus_masterrtu.h"

Modbus_MasterRtu::Modbus_MasterRtu(QObject *parent) : Modbus_MasterObj{parent}
{
    mModbus = new QModbusRtuSerialMaster(nullptr);
}

bool Modbus_MasterRtu::connectModbus(const QString &name, int baud)
{    
    initConnects();
    initModbusSerial(name, baud);
    mModbus->setNumberOfRetries(mSet.numberOfRetries);
    mModbus->setTimeout(mSet.responseTime);
    return connectDevice();
}
