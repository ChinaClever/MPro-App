/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_output.h"
#include "op_core.h"
#include "cascade_core.h"

Set_Output::Set_Output()
{

}

bool Set_Output::outputRelayCtrl(int addr, int id, uchar on)
{
    bool ret = true;
    if(addr) {
        ret = Cascade_Core::bulid()->masterRelayCtrl(addr, id, on);
    } else {
        OP_Core::bulid()->relayCtrl(id, on);
    }

    return ret;
}

bool Set_Output::outputDelaySet(int addr, int id, uchar delay)
{
    bool ret = true;
    if(addr) {
        ret = Cascade_Core::bulid()->masterDelaySet(addr, id, delay);
    } else {
        OP_Core::bulid()->setDelay(id, delay);
        sRelayUnit *it = &(cm::masterDev()->output.relay);
        if(id) it->delay[id-1] = delay;
        else for(int i=0; i<it->size; ++i) it->delay[i] = delay;
        Set_ReadWrite::bulid()->writeSettings();
    }
    return ret;
}


bool Set_Output::outputSwModeSet(int addr, int id, uchar mode)
{
    bool ret = true;
    if(addr) {
        ret = Cascade_Core::bulid()->masterSwModeSet(addr, id, mode);
    } else {
        sRelayUnit *it = &(cm::masterDev()->output.relay);
        if(id) it->mode[id-1] = mode;
        else for(int i=0; i<it->size; ++i) it->mode[i] = mode;
        Set_ReadWrite::bulid()->writeSettings();
    }

    return ret;
}

bool Set_Output::outputNameSet(int addr, int id, const QString &name)
{
    bool ret = true;
    if(addr) {
        ret = Cascade_Core::bulid()->masterOpNameSet(addr, id, name);
    } else {
        sObjData *it = &(cm::masterDev()->output);
        if(id) qstrcpy((char *)it->name[id-1], name.toLatin1().data());
        else for(int i=0; i<it->size; ++i) qstrcpy((char *)it->name[i], name.toLatin1().data());
        Set_ReadWrite::bulid()->writeSettings();
    }
    return ret;
}
