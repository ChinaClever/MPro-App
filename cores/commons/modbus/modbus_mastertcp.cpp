/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "modbus_mastertcp.h"

Modbus_MasterTcp::Modbus_MasterTcp(QObject *parent) : Modbus_MasterObj{parent}
{
    mModbus = new QModbusTcpClient(this);
}

bool Modbus_MasterTcp::connectModbus(const QString &url, int port)
{
    initConnects();
    initModbusNet(url, port);
    mModbus->setNumberOfRetries(mSet.numberOfRetries);
    mModbus->setTimeout(mSet.responseTime);
    return connectDevice();
}
