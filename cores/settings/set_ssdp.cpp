/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_ssdp.h"
#include "integr_jsonrecv.h"

Set_Ssdp::Set_Ssdp(QObject *parent)
    : Ssdp_Obj{parent}
{
    Integr_JsonRecv *j = Integr_JsonRecv::bulid(this); ssdpBind();
    connect(j, &Integr_JsonRecv::recvSetSig, this, &Set_Ssdp::recvSetSlot);
    connect(j, &Integr_JsonRecv::recvCfgSig, this, &Set_Ssdp::recvCfgSlot);
    connect(this, &Ssdp_Obj::recvSig, this, &Set_Ssdp::recvSlot);
}

Set_Ssdp *Set_Ssdp::bulid(QObject *parent)
{
    static Set_Ssdp* sington = nullptr;
    if(!sington) sington = new Set_Ssdp(parent);
    return sington;
}

bool Set_Ssdp::setting(sDataItem &it, const QString &room, const QString &ip)
{
    sSdpIt item; item.fc = 2; item.room = room; item.ip = ip;
    item.data = cm::toByteArray(it);
    return ssdpSend(item);
}

bool Set_Ssdp::setCfg(sCfgItem &it, const QVariant &v, const QString &room, const QString &ip)
{
    sSdpIt item; item.fc = 3; item.room = room; item.ip = ip;
    QByteArray array; QDataStream in(&array, QIODevice::WriteOnly);
    in << cm::toByteArray(it) << v;
    item.data = array;
    return ssdpSend(item);
}

bool Set_Ssdp::setJson(const QByteArray &data, const QString &room, const QString &ip)
{
    sSdpIt item; item.fc = 1; item.data = data;
    item.room = room; item.ip = ip;
    return ssdpSend(item);
}

bool Set_Ssdp::checkInput(const sSdpIt &it)
{
    bool ret = true;
    if(it.room.size()) {
        QString res = cm::masterDev()->cfg.uut.room;
        if(res != it.room) ret = false;
    }

    if(it.ip.size()) {
        QString res = cm::dataPacket()->net.inet.ip;
        if(res != it.ip) ret = false;
    }

    return ret;
}

void Set_Ssdp::recvSwVersion()
{
    sSdpIt item; item.fc = 11;
    item.ip = cm::dataPacket()->net.inet.ip;
    item.room = cm::masterDev()->cfg.uut.room;
    QString fmt = "ver:%1; date:%2; usr:%3; md5:%4; Outlet:";

    int num = cm::masterDev()->cfg.nums.slaveNum;
    for(int i=0; i<=num; i++) {
        sVersions *ver = &(cm::devData(i)->cfg.vers);
        QString str = fmt.arg(ver->ver, ver->releaseDate, ver->usr, ver->md5);
        for (int k = 0; k < 6; ++k) {
            str += QString::number(ver->opVers[k]/10.0, 'f',1) +"; ";
        }
        item.data = str.toUtf8();
        udpSend(item);
    }
}

void Set_Ssdp::recvSlot(const sSdpIt &it)
{
    if(!checkInput(it)) return;

    if(it.fc ==1) {
        Integr_JsonRecv::bulid()->recv(it.data);
    } else if(it.fc == 2) {
        sDataItem unit = cm::toStruct<sDataItem>(it.data);
        Set_Core::bulid()->setting(unit);
    } else if(it.fc == 3) {
        QByteArray msg(it.data), array; QVariant value;
        QDataStream out(&msg, QIODevice::ReadOnly);
        out >> array >> value;
        sCfgItem unit = cm::toStruct<sCfgItem>(array);
        Set_Core::bulid()->setCfg(unit, value);
    } else if(it.fc == 11) {
        recvSwVersion();
    } else cout << it.fc;
}

void Set_Ssdp::recvCfgSlot(const sCfgItem &it, const QVariant &v)
{
    sCfgItem unit = it;
    Set_Core::bulid()->setCfg(unit, v);
}

void Set_Ssdp::recvSetSlot(const sDataItem &it)
{
    sDataItem unit = it;
    Set_Core::bulid()->setting(unit);
}

bool Set_Ssdp::rplySearchTarget(const sSdpIt &it)
{
    bool ret = checkInput(it);
    if(ret && it.data.size() > 100) {
        sSdpIt item; item.fc = 0;
        item.ip = cm::dataPacket()->net.inet.ip;
        item.room = cm::masterDev()->cfg.uut.room;
        item.data = QByteArray::number(cm::masterDev()->cfg.nums.slaveNum);
        ret = udpSend(item);
    }
    return ret;
}
