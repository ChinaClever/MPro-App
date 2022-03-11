#include "modbus_slavertu.h"

Modbus_SlaveRtu::Modbus_SlaveRtu(QObject *parent) : Modbus_SlaveTcp{parent}
{

}


bool Modbus_SlaveRtu::connectModbus(const QString &name, int baud)
{
    initConnects();
    initModbusSerial(name, baud);
    return connectDevice();
}

bool Modbus_SlaveRtu::connectRtu(const QString &name, int baud, int addr)
{
    bool ret = true;
    if(mDev) {
        disconnectModbus();
    } else {
        mDev = new QModbusRtuSerialSlave(this);
        ret = initUnitMap();
    }

    if(ret) setAddress(addr);
    if(ret) ret = connectModbus(name, baud);
    return ret;
}
