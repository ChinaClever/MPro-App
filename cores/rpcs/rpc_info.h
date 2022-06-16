#ifndef RPC_INFO_H
#define RPC_INFO_H

#include "rpc_obj.h"

class Rpc_Info : public Rpc_Obj
{
public:
    explicit Rpc_Info(QObject *parent = nullptr);

    Q_INVOKABLE QString pduDevNameGet(int addr);
    Q_INVOKABLE bool pduDevNameSet(int addr, const QString &value);

    Q_INVOKABLE QString pduDevSnGet(int addr);
    Q_INVOKABLE bool pduDevSnSet(int addr, const QString &value);

};

#endif // RPC_INFO_H
