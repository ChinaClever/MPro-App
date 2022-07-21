#ifndef RPC_OUTPUT_H
#define RPC_OUTPUT_H

#include "rpc_info.h"

class Rpc_Output : public Rpc_Info
{
public:
    explicit Rpc_Output(QObject *parent = nullptr);
    Q_INVOKABLE uint pduRelayStatus(int addr, int id);
    Q_INVOKABLE bool pduRelayCtrl(int addr, int id, uchar on);
    Q_INVOKABLE bool pduGroupCtrl(int addr, int id, uchar on);
    Q_INVOKABLE bool pduRelayModeSet(int addr, int id, uchar mode);
    Q_INVOKABLE bool pduRelayDelaySet(int addr, int id, uchar delay);
    Q_INVOKABLE bool pduRelaysCtrl(int addr, int start, int num, uchar on);
    Q_INVOKABLE bool pduGroupingSet(int addr, int id, const QString &value);
    Q_INVOKABLE bool pduGroupNameSet(int addr, int id, const QString &value);
    Q_INVOKABLE bool pduOutputNameSet(int addr, int id, const QString &value);    
    Q_INVOKABLE QString pduOutputNameGet(int addr, int id);
    Q_INVOKABLE QString pduGroupNameGet(int addr, int id);
    Q_INVOKABLE QString pduGroupingGet(int addr, int id);
};

#endif // RPC_OUTPUT_H
