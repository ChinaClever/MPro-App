#ifndef MODBUS_SLAVEOBJ_H
#define MODBUS_SLAVEOBJ_H

#include "modbus_mastertcp.h"

class Modbus_SlaveObj : public Modbus_Object
{
    Q_OBJECT
public:
    explicit Modbus_SlaveObj(QObject *parent = nullptr);

    void setAddress(int addr) {mDev->setServerAddress(addr);}
    bool setCoils(quint16 address, const QVector<quint16> &values);
    bool setHoldingRegisters(quint16 address, const QVector<quint16> &values);

signals:
    //void registerDataSig(int address,int value);
    void rcvDataSig(int address, QVector<quint16> values);

protected slots:
    void recvDataSlot(QModbusDataUnit::RegisterType table, int address, int size);

protected:
    void initConnects();
    bool initUnitMap();
    QModbusDevice *modbusDevice() override {return mDev;}

private:
    bool setData(const QModbusDataUnit &unit);
    bool setData(quint16 address, const QVector<quint16> &values);
    bool setData(QModbusDataUnit::RegisterType table, quint16 address, quint16 data);

protected:
    QModbusServer* mDev;
};

#endif // MODBUS_SLAVEOBJ_H
