#include "modbus_slavertu.h"

Modbus_SlaveRtu::Modbus_SlaveRtu(QObject *parent) : Modbus_SlaveObj{parent}
{
    mDev = new QModbusRtuSerialSlave(this);
}

bool Modbus_SlaveRtu::connectModbus(const QString &name, int baud)
{
    initConnects();
    initModbusSerial(name, baud);
    return connectDevice();
}

bool Modbus_SlaveRtu::connectModbus(const QString &name, int baud, int addr, quint16 size)
{
    bool ret = initUnitMap(size);
    if(ret) setAddress(addr);
    if(ret) ret = connectModbus(name, baud);
    return ret;
}
