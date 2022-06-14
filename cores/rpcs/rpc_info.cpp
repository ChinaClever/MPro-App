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
     return pduSetString(addr, SFnCode::Uuts, 6, value);
}

QString Rpc_Info::pduDevNameGet(int addr)
{
    return pduGetString(addr, SFnCode::Uuts, 6);
}

QString Rpc_Info::pduDevSnGet(int addr)
{
    return pduGetString(addr, SFnCode::Uuts, 7);
}

bool Rpc_Info::pduDevSnSet(int addr, const QString &value)
{
    return pduSetString(addr, SFnCode::Uuts, 7, value);
}
