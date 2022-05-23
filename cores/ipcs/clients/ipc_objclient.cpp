/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ipc_objclient.h"

IPC_ObjClient::IPC_ObjClient(QObject *parent)
    : QObject{parent}
{
    mDbus = nullptr;
}

sDataPacket *IPC_ObjClient::dataPacket()
{
    sDataPacket *res = nullptr;
    if(mDbus) res = (sDataPacket *)mDbus->sharedMemory();
    return res;
}

void IPC_ObjClient::initFunction(const QString &key, bool f)
{
    if(!mDbus) mDbus = new DBus_Call(key, this);
    if(f) QTimer::singleShot(1, mDbus, SLOT(lscReconnect()));
}

QVariant IPC_ObjClient::readBus(const QVariantList &v)
{
    QVariant res;
    if(inputCheck(v)) {
        QVariantList lv = mDbus->callBus(v);
        if(lv.size()) res = lv.first();
    }
    return  res;
}

bool IPC_ObjClient::msgSend(int fc, const QByteArray &msg)
{
    QVariantList lv{fc, msg};
    bool ret = inputCheck(lv);
    if(ret) ret = mDbus->sendBus(lv);
    return ret;
}

bool IPC_ObjClient::setString(const sStrItem &unit)
{
    QByteArray array = cm::toByteArray(unit);
    return msgSend(2, array);
}

bool IPC_ObjClient::setString(uchar addr, uchar fc, uchar id, const QString &str)
{
    sStrItem it; it.addr = addr; it.fc = fc; it.id = id; it.rw = 1;
    qstrcpy((char *)it.str, str.toLatin1().data());
    return this->setString(it);
}

bool IPC_ObjClient::setting(const sDataItem &unit)
{
    QByteArray array = cm::toByteArray(unit);
    return msgSend(1, array);
}

int IPC_ObjClient::getValue(sDataItem &unit)
{
    unit.value = (int)-1;
    QByteArray array = cm::toByteArray(unit);
    QString res = readBus(QVariantList {1, array}).toString();
    if(res.size()) unit.value = res.toUInt();
    return unit.value;
}

bool IPC_ObjClient::setting(uchar addr, uchar type, uchar topic, uchar sub, uchar id, uint value)
{
    sDataItem it; it.addr = addr; it.type = type;
    it.topic = topic; it.subtopic = sub; it.id = id;
    it.rw = 1; it.value = value;
    return this->setting(it);
}

int IPC_ObjClient::getValue(uchar addr, uchar type, uchar topic, uchar sub, uchar id)
{
    sDataItem it; it.addr = addr; it.type = type;
    it.topic = topic; it.subtopic = sub; it.id = id;
    return this->getValue(it);
}
