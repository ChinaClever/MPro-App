#ifndef MODBUS_SLAVERTU_H
#define MODBUS_SLAVERTU_H

#include "modbus_slaveobj.h"

class Modbus_SlaveRtu : public Modbus_SlaveObj
{
    Q_OBJECT
public:
    explicit Modbus_SlaveRtu(QObject *parent = nullptr);
    bool connectModbus(const QString &name, int baud, int addr, quint16 size);
protected:
    bool connectModbus(const QString &name, int baud);
};

#endif // MODBUS_SLAVERTU_H
