#ifndef RPC_OUTPUT_H
#define RPC_OUTPUT_H

#include "rpc_info.h"

class Rpc_Output : public Rpc_Info
{
public:
    explicit Rpc_Output(QObject *parent = nullptr);
    Q_INVOKABLE uint pduRelayStatus(int addr, int id);
    Q_INVOKABLE bool pduRelayCtrl(int addr, int id, uchar on);

    Q_INVOKABLE QString pduOutputNameGet(int addr, int id);
    Q_INVOKABLE bool pduOutputNameSet(int addr, int id, const QString &value);
};

#endif // RPC_OUTPUT_H
