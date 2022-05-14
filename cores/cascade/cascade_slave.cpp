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


bool Cascade_Slave::replyAlarm(QByteArray &rcv)
{
    sSetAlarmUnit unit = cm::toStruct<sSetAlarmUnit>(rcv);
    unit.index.addr = 0; Set_Core *set = Set_Core::bulid();
    return set->setAlarm(unit);
}

bool Cascade_Slave::replyRelayCtrl(QByteArray &rcv)
{
    Set_Core *set = Set_Core::bulid();
    return set->outputRelayCtrl(0, rcv[0], rcv[1]);
}

bool Cascade_Slave::replyRelaySet(QByteArray &rcv)
{
    Set_Core *set = Set_Core::bulid();
    return set->outputDelaySet(0, rcv[0], rcv[1], rcv[2]);
}

bool Cascade_Slave::workDown(c_sFrame &it)
{
    bool ret = false;
    if((it.dstAddr == getAddress()) || (it.dstAddr == fc_mask)) {
        switch (it.fc) {
        case fc_readDev: ret = replyDevData(it.fc); break;
        case fc_writeAlarm: ret = replyAlarm(it.data); break;
        case fc_delaySet: ret = replyRelaySet(it.data); break;
        case fc_relayCtrl: ret = replyRelayCtrl(it.data); break;

        case fc_otaStart: ret = otaReplyStart(it.data); break;
        case fc_otaEnd: ret = otaReplyFinish(it.data); break;
        case fc_otaPack: ret = otaReplyPacket(it.data); break;
        }
    }

    return ret;
}

