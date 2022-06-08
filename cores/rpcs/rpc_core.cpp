/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "rpc_core.h"

Rpc_Core::Rpc_Core(uchar txType, QObject *parent)
    : Rpc_Output{parent}
{
    setTxType(txType);
}
