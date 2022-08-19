/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "data_relay.h"

Data_Relay::Data_Relay()
{

}


bool Data_Relay::relayOverrunOff(const sObjData &obj, int id)
{
    bool ret = false;
    const sRelayUnit *it = &obj.relay;
    if((id < it->size) && it->overrunOff[id]) {
        if(obj.cur.value[id] > obj.cur.max[id]) {
            if(obj.pow.en[id] || obj.cur.en[id])
                ret = true;
        }
    }

    return ret;
}


int Data_Relay::relayTiming(const sObjData &obj, int id)
{
    int ret = 0;
    const sRelayUnit *it = &obj.relay;
    if((id < it->size) && it->timingEn[id]) {
        ret = relayTimingCheck(it->timingOn, id) ? 2:0;
        if(!ret) ret = relayTimingCheck(it->timingOff, id) ? 1:0;
    }
    return ret;
}


bool Data_Relay::relayTimingCheck(const char (*str)[NAME_SIZE], int id)
{
    bool ret = false;
    const char *ptr = str[id];
    QString tcmd = "hh:mm:ss";
    QString dtcmd = "yyyy-MM-dd " + tcmd;

    if(strlen(ptr) == 8) {
        QTime t = QTime::fromString(ptr, tcmd);
        if(t == QTime::currentTime()) ret = true;
    } else if(strlen(ptr) == 19) {
        QDateTime t = QDateTime::fromString(ptr, dtcmd);
        if(t == QDateTime::currentDateTime()) ret = true;
    }

    return ret;
}
