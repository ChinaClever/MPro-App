/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_ssdp.h"
#include "integr_jsonrecv.h"

Set_Ssdp::Set_Ssdp(QObject *parent)
    : QObject{parent}
{
    mSsdp = new Ssdp_Server(this);
    Ssdp_Client *c = Ssdp_Client::bulid(this);
    connect(c, &Ssdp_Client::recvSig, this, &Set_Ssdp::recvSlot);

    Integr_JsonRecv *j = Integr_JsonRecv::bulid(this);
    connect(j, &Integr_JsonRecv::recvSetSig, this, &Set_Ssdp::recvSetSlot);
    connect(j, &Integr_JsonRecv::recvCfgSig, this, &Set_Ssdp::recvNumStrSlot);
}

Set_Ssdp *Set_Ssdp::bulid(QObject *parent)
{
    static Set_Ssdp* sington = nullptr;
    if(!sington) sington = new Set_Ssdp(parent);
    return sington;
}

void Set_Ssdp::recvSlot(uchar fc, const QString &room, const QByteArray&rcv)
{
    if(room.size()) {
        QString res = cm::masterDev()->cfg.uut.room;
        if(res != room) return ;
    }

    if(fc == 1) {
        sDataItem unit = cm::toStruct<sDataItem>(rcv);
        Set_Core::bulid()->setting(unit);
    } else if(fc == 2) {
        sCfgItem unit = cm::toStruct<sCfgItem>(rcv);
        Set_Core::bulid()->setCfg(unit);
    } else qDebug() << Q_FUNC_INFO << fc;
}

void Set_Ssdp::recvNumStrSlot(const sCfgItem &it)
{
    sCfgItem unit = it;
    Set_Core::bulid()->setCfg(unit);
}

void Set_Ssdp::recvSetSlot(const sDataItem &it)
{
    sDataItem unit = it;
    Set_Core::bulid()->setting(unit);
}
