#ifndef MODBUS_SLAVETCP_H
#define MODBUS_SLAVETCP_H
#include "modbus_slaveobj.h"

class Modbus_SlaveTcp : public Modbus_SlaveObj
{
    Q_OBJECT
public:
    explicit Modbus_SlaveTcp(QObject *parent = nullptr);
    bool connectTcp(int port=502, const QString &url="127.0.0.1");

private:
    bool connectModbus(const QString &url, int port);
};

#endif // MODBUS_SLAVETCP_H
