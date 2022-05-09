#include "mb_core.h"

Mb_Core::Mb_Core(QObject *parent)
{
    mRtu = new Mb_Restore(parent);
    mTcp = new Mb_Restore(parent);
}


Mb_Core *Mb_Core::bulid(QObject *parent)
{
    static Mb_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new Mb_Core(parent);
        sington->connectTcp();
    }
    return sington;
}

bool Mb_Core::connectTcp(int port)
{
    bool ret = mTcp->isConnectedModbus();
    if(!ret) ret = mTcp->connectTcp(port);
    return ret;
}

bool Mb_Core::connectRtu(int addr, int baud, const QString &name)
{
    bool ret = mRtu->isConnectedModbus();
    if(!ret) {
       ret = mRtu->connectRtu(name, baud, addr);
    }
    return ret;
}

void Mb_Core::run()
{
    bool ret = true;
    while (isRun) {
        ret = mRtu->isConnectedModbus();
        cm::mdelay(500); if(ret) mRtu->mbUpdates();

        ret = mTcp->isConnectedModbus();
        cm::mdelay(500); if(ret) mTcp->mbUpdates();
    }
}
