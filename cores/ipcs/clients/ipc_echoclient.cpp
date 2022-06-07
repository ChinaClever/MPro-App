/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ipc_echoclient.h"

IPC_EchoClient::IPC_EchoClient(QObject *parent)
    : IPC_ObjClient{parent}
{

}

bool IPC_EchoClient::msgSend(int fc, const QByteArray &msg)
{
    QVariantList lv{fc, msg};
    bool ret = inputCheck(lv);
    if(ret) ret = mDbus->sendBus(lv);
    return ret;
}

bool IPC_EchoClient::setString(const sStrItem &unit)
{
    QByteArray array = cm::toByteArray(unit);
    return msgSend(2, array);
}

bool IPC_EchoClient::setString(uchar addr, uchar fc, uchar id, const QString &str)
{
    sStrItem it; it.addr = addr; it.fc = fc; it.id = id; it.rw = 1;
    qstrcpy((char *)it.str, str.toLatin1().data());
    return this->setString(it);
}

bool IPC_EchoClient::setting(const sDataItem &unit)
{
    QByteArray array = cm::toByteArray(unit);
    return msgSend(1, array);
}

bool IPC_EchoClient::getValue(sDataItem &unit)
{
    bool ret = true; QByteArray array = cm::toByteArray(unit);
    QString res = readBus(QVariantList {1, array}).toString();
    if(res.size()) unit.value = res.toUInt(); else ret = false;
    return ret;
}

QString IPC_EchoClient::getString(sStrItem &unit)
{
    QByteArray array = cm::toByteArray(unit);
    QString res = readBus(QVariantList {2, array}).toString();
    return res;
}

QString IPC_EchoClient::getString(uchar addr, uchar fc, uchar id)
{
    sStrItem it; it.addr = addr;
    it.fc = fc; if(id) id--;
    it.id = id;
    return getString(it);
}

bool IPC_EchoClient::setting(uchar addr, uchar type, uchar topic, uchar sub, uchar id, uint value)
{
    sDataItem it; it.addr = addr; it.type = type;
    it.topic = topic; it.subtopic = sub; it.id = id;
    it.rw = 1; it.value = value;
    qDebug()<<addr<<":"<<type<<":"<<topic<<":"<<sub<<":"<<id<<":"<<value;
    return this->setting(it);
}

int IPC_EchoClient::getValue(uchar addr, uchar type, uchar topic, uchar sub, uchar id)
{
    sDataItem it; it.addr = addr; it.type = type; if(id) id--;
    it.topic = topic; it.subtopic = sub; it.id = id;
    bool ret = this->getValue(it); int res = -1;
    if(ret) res = it.value;

    return res;
}
