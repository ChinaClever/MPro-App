#ifndef RPC_CORE_H
#define RPC_CORE_H

#include "rpc_read.h"

class Rpc_Core : public JsonRpcObj
{
    Q_OBJECT
    explicit Rpc_Core(QObject *parent = nullptr);
public:
    static Rpc_Core *bulid(QObject *parent = nullptr);



private slots:
    void initFunSlot();

};

#endif // RPC_CORE_H
