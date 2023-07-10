/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "data_relay.h"
#include "cfg_core.h"

Data_Relay::Data_Relay()
{

}


bool Data_Relay::relayOverrunOff(const sObjData &obj, int id)
{
    bool ret = false;
    const sRelayUnit *it = &obj.relay;
    if((obj.cur.value[id] < COM_MAX_CUR) && (obj.pow.value[id] < COM_MAX_POW)) {
        if((id < it->size) && it->overrunOff[id]) {
            if((obj.cur.value[id] > obj.cur.max[id]) || (obj.pow.value[id] > obj.pow.max[id])) {
                if(obj.pow.en[id] || obj.cur.en[id]) ret = true;
            }
        }
    }

    return ret;
}


int Data_Relay::relayTiming(sObjData &obj, int id)
{
    int ret = 0;
    const sRelayUnit *it = &obj.relay;
    if((id < it->size) && it->timingEn[id] ) {
        ret = relayTimingCheck(it->timingOff, id) ? 1:0;
        if(!ret) {
            ret = relayTimingCheck(it->timingOn, id) ? 2:0;
            if(ret) {obj.relay.cnt[id] += 1; Cfg_Core::bulid()->writeAlarms();}
        }
    }
    return ret;
}


bool Data_Relay::relayTimingCheck(const char (*str)[NAME_SIZE], int id)
{
    bool ret = false;
    const char *ptr = str[id];
    QString tcmd = "hh:mm"; // :ss
    QString dtcmd = "yyyy-MM-dd " + tcmd;

    if((int)strlen(ptr) == tcmd.size()) {
        QTime t = QTime::fromString(ptr, tcmd);
        if(t.toString() == QTime::currentTime().toString()) ret = true;
   } else if((int)strlen(ptr) == dtcmd.size()) {
        QDateTime t = QDateTime::fromString(ptr, dtcmd);
        if(t.toString() == QDateTime::currentDateTime().toString()) ret = true;
    } else if(strlen(ptr)) cout << ptr;

    return ret;
}
