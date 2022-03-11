#include "modbus_slavetcp.h"

Modbus_SlaveTcp::Modbus_SlaveTcp(QObject *parent) : Modbus_SlaveObj{parent}
{
    mDev = new QModbusTcpServer(this);
}


bool Modbus_SlaveTcp::connectModbus(const QString &url, int port)
{
    initConnects();
    initModbusNet(url, port);
    return connectDevice();
}

bool Modbus_SlaveTcp::connectModbus(int port, const QString &url)
{
    setAddress(1);
    initUnitMap();
    return connectModbus(url, port);
}
