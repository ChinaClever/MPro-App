/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cascade_slave.h"

Cascade_Slave::Cascade_Slave(QObject *parent) : Cascade_Fill{parent}
{
}


bool Cascade_Slave::replyDevData(uchar fc)
{
    fillData(getAddress());
    QByteArray array = toDataStream();
    return writeData(fc, 0, array);
}

bool Cascade_Slave::replySet(QByteArray &rcv)
{
    sDataItem unit = cm::toStruct<sDataItem>(rcv);
    unit.addr = 0; Set_Core *set = Set_Core::bulid();
    return set->setting(unit);
}

bool Cascade_Slave::replyNumStr(QByteArray &rcv)
{
    sNumStrItem unit = cm::toStruct<sNumStrItem>(rcv);
    unit.addr = 0; Set_Core *set = Set_Core::bulid();
    return set->setNumStr(unit);
}


bool Cascade_Slave::workDown(c_sFrame &it)
{
    bool ret = false;
    if((it.dstAddr == getAddress()) || (it.dstAddr == fc_mask)) {
        switch (it.fc) {
        case fc_readDev: ret = replyDevData(it.fc); break;
        case fc_setting: ret = replySet(it.data); break;
        case fc_setNumStr: ret = replyNumStr(it.data); break;

        case fc_otaStart: ret = otaReplyStart(it.data); break;
        case fc_otaEnd: ret = otaReplyFinish(it.data); break;
        case fc_otaPack: ret = otaReplyPacket(it.data); break;
        }
    }

    return ret;
}

