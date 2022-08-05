/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ipc_webclient.h"

IPC_WebClient::IPC_WebClient(QObject *parent)
    : IPC_LogClient{parent}
{

}

IPC_WebClient *IPC_WebClient::bulid(QObject *parent)
{
    static IPC_WebClient *sington = nullptr;
    if(!sington) {
        sington = new IPC_WebClient(parent);
    }
    return sington;
}

bool IPC_WebClient::msgSend(int fc, const QByteArray &msg)
{
    QVariantList lv{fc, msg};
    bool ret = inputCheck(lv);
    if(ret) ret = sendSocket(lv);
    return ret;
}

bool IPC_WebClient::setCfg(uint addr, uchar fc, uchar type, const QVariant &value)
{
    sCfgItem it; it.txType = DTxType::TxWeb; it.fc = fc;
    it.addr = (0xFF & addr); it.type = type; // it.value = value;
    QByteArray array; QDataStream in(&array, QIODevice::WriteOnly);
    in << cm::toByteArray(it) << value;
    return msgSend(6, array);
}

bool IPC_WebClient::setting(const sDataItem &unit)
{
    QByteArray array = cm::toByteArray(unit);
    return msgSend(1, array);
}

bool IPC_WebClient::getValue(sDataItem &unit)
{
    bool ret = true; QByteArray array = cm::toByteArray(unit);
    QString res = readSocket(QVariantList {1, array}).toString();
    if(res.size()) unit.value = res.toUInt(); else ret = false;
    return ret;
}

QString IPC_WebClient::getCfg(uchar addr, uchar fc, uchar id)
{
    sCfgItem it; it.addr = addr; it.fc = fc; it.type = id;
    QByteArray array = cm::toByteArray(it);
    return readSocket(QVariantList {5, array}).toString();
}

bool IPC_WebClient::setting(uint addr, uchar type, uchar topic, uchar sub, uchar id, uint value)
{
    sDataItem it; it.type = type;
    it.addr = (0xFF & addr); //it.soi = addr >> 8;
    it.topic = topic; it.subtopic = sub; it.id = id;
    it.rw = 1; it.value = value; it.txType = DTxType::TxWeb;
    //qDebug()<<addr<<":"<<type<<":"<<topic<<":"<<sub<<":"<<id<<":"<<value;
    return this->setting(it);
}

int IPC_WebClient::getValue(uchar addr, uchar type, uchar topic, uchar sub, uchar id)
{
    sDataItem it; it.addr = addr; it.type = type; if(id) id--;
    it.topic = topic; it.subtopic = sub; it.id = id;
    bool ret = this->getValue(it); int res = -1;
    if(ret) res = it.value;

    return res;
}
