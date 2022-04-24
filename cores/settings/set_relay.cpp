/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_relay.h"
#include "op_zrtu.h"
#include "cascade_slave.h"

Set_Relay::Set_Relay()
{

}

bool Set_Relay::outputRelayCtrl(int addr, int id, uchar on)
{
    bool ret = true;
    if(addr) {
       ret = Cascade_Core::bulid()->masterRelayCtrl(addr, id, on);
    } else {
        OP_Core::bulid()->relayCtrl(id, on);
    }

    return ret;
}

bool Set_Relay::outputDelaySet(int addr, int id, uchar mode, uchar delay)
{
    bool ret = true;
    if(addr) {
        ret = Cascade_Core::bulid()->masterDelaySet(addr, id, mode, delay);
    } else {
        OP_ZRtu::bulid()->setDelay(id, delay);
        sRelayUnit *it = &(cm::masterDev()->output.relay);
        if(id) {
            it->mode[id-1] = mode; it->delay[id-1] = delay;
        } else {
            for(int i=0; i<it->size; ++i) {it->mode[i] = mode; it->delay[i] = delay;}
        }
        Set_readWrite::bulid()->writeSettings();
    }

    return ret;
}
