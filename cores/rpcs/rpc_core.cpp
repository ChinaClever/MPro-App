#include "rpc_core.h"
#include <QTimer>

Rpc_Core::Rpc_Core(QObject *parent)
    : JsonRpcObj{parent}
{
    QTimer::singleShot(550,this,SLOT(initFunSlot()));
}


Rpc_Core *Rpc_Core::bulid(QObject *parent)
{
    static Rpc_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new Rpc_Core(parent);
    }
    return sington;
}

void Rpc_Core::initFunSlot()
{
    auto service1 = new Rpc_Read;
    startServer({service1});
}
