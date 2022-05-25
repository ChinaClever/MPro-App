#ifndef RPC_SERVICE_H
#define RPC_SERVICE_H

#include "rpc_core.h"

class Rpc_Service : public JsonRpcObj
{
    Q_OBJECT
    explicit Rpc_Service(QObject *parent = nullptr);
public:
    static Rpc_Service *bulid(QObject *parent = nullptr);



private slots:
    void initFunSlot();

};

#endif // RPC_SERVICE_H
