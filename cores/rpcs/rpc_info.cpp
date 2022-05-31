/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "rpc_info.h"

Rpc_Info::Rpc_Info(QObject *parent)
    : Rpc_Obj{parent}
{

}


bool Rpc_Info::pduDevNameSet(int addr, const QString &value)
{
     return pduSetString(addr, 11, 6, value);
}

QString Rpc_Info::pduDevNameGet(int addr)
{
    return pduGetString(addr, 11, 6);
}
