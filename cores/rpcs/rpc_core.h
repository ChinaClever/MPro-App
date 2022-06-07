#ifndef RPC_CORE_H
#define RPC_CORE_H
#include "rpc_output.h"

class Rpc_Core : public Rpc_Output
{
public:
    Rpc_Core(uchar txType, QObject *parent = nullptr);

};

#endif // RPC_CORE_H
