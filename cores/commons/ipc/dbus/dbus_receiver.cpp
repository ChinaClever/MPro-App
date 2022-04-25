/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "dbus_receiver.h"

DBus_Receiver::DBus_Receiver(QObject *parent) : DBus_Call{parent}
{
}

bool DBus_Receiver::busConnects()
{
    bool ret = true;
    QList<const char *> ls = busRecvMethods();
    foreach(auto &i, ls) {
        if(i) ret = mBus.connect(mService, mBusPath, mInterface, mBusSig, this, i);
        if(ret) ret = mBus.isConnected(); else throwError(Q_FUNC_INFO + mInterface);
    }

    return ret;
}

