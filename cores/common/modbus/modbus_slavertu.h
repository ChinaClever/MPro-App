#ifndef MODBUS_SLAVERTU_H
#define MODBUS_SLAVERTU_H
#include "modbus_slavetcp.h"


class Modbus_SlaveRtu : public Modbus_SlaveTcp
{
    Q_OBJECT
public:
    explicit Modbus_SlaveRtu(QObject *parent = nullptr);
    bool connectRtu(const QString &name, int baud, int addr);

private:
    bool connectModbus(const QString &name, int baud);
};

#endif // MODBUS_SLAVERTU_H
