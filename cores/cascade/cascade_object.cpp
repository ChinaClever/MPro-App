/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cascade_object.h"

Cascade_Object::Cascade_Object(QObject *parent) : SerialPort{parent}
{
    mCData = new c_sDevData;
    mDataStream = new c_DataStream(mCData);
    memset((void *)mCData, 0, sizeof(c_sDevData));
}

void Cascade_Object::setAddress()
{
    sParameter *p = &(cm::masterDev()->cfg.param);
    uchar addr = 1; if(p->devMode < DevMode::DM_Rtu) addr = p->cascadeAddr;
    mSelfAddr = addr;
}

QByteArray Cascade_Object::frameToArray(const c_sFrame &it)
{
    QByteArray array; QDataStream in(&array, QIODevice::WriteOnly);
    in << START_HEAD << it.dstAddr << mSelfAddr /*it.srcAddr*/ << it.fc << it.len;
    if(it.len) {in << it.data;} in << END_CRC;
    return array;
}

bool Cascade_Object::arrayToFrame(QDataStream &out, c_sFrame &it)
{
    ushort head, crc; bool ret = false; out >> head;
    if(head == START_HEAD) out >> it.dstAddr >> it.srcAddr >> it.fc >> it.len;
    if(it.len) {out >> it.data;} out >> crc; // Crc::Cal(array)
    if(crc == END_CRC) ret = true;
    return ret;
}

QVector<c_sFrame> Cascade_Object::arrayToFrames(QByteArray &array)
{
    QVector<c_sFrame> its;
    QDataStream out(&array, QIODevice::ReadOnly);
    if((array.size()>6) && crcCheck(array)) {
        while(!out.atEnd()) {
            c_sFrame it; bool ret = arrayToFrame(out, it);
            if(ret) its << it; else cout;
        }
    }

    return its;
}

QVector<c_sFrame> Cascade_Object::replyData(QByteArray &rcv, uchar addr, uchar fc)
{
    QVector<c_sFrame> res;
    QVector<c_sFrame> its = arrayToFrames(rcv);
    foreach(const auto &it, its) {
        if(it.srcAddr == addr) {
            if(fc) if(fc != it.fc) continue;
            res << it;
        }
    }

    return res;
}

QVector<c_sFrame> Cascade_Object::readData(uchar fc, uchar addr)
{
    c_sFrame it; it.fc = fc; it.dstAddr = addr; it.len=0;
    QByteArray array = frameToArray(it);
    array = transmit(qCompress(array));
    if(array.size()) array = qUncompress(array);
    return arrayToFrames(array);
}

bool Cascade_Object::writeData(uchar fc, uchar addr, const QByteArray &value)
{
    c_sFrame it; it.fc = fc; it.dstAddr = addr;
    it.len = value.size(); it.data = value;
    QByteArray array = frameToArray(it);
    return writeSerial(qCompress(array));
}

QVector<c_sFrame> Cascade_Object::transData(uchar fc, uchar addr, const QByteArray &value)
{
    c_sFrame it; it.fc = fc; it.dstAddr = addr;
    it.len = value.size(); it.data = value;
    QByteArray array = frameToArray(it);
    array = transmit(qCompress(array), 1654);
    if(array.size()) array = qUncompress(array);
    return replyData(array, addr, fc);
}

QByteArray Cascade_Object::tranData(uchar fc, uchar addr, const QByteArray &value)
{
    QByteArray array;
    QVector<c_sFrame> res = transData(fc, addr, value);
    if(res.size()) array = res.takeLast().data;
    return array;
}

QByteArray Cascade_Object::toDataStream()
{
    QByteArray array;
    QDataStream in(&array, QIODevice::WriteOnly);
    in << *mDataStream;
    return array;
}

QByteArray Cascade_Object::toDataStream(c_sDevData *data)
{
    QByteArray array; c_DataStream dataStream(data);
    QDataStream in(&array, QIODevice::WriteOnly);
    in << dataStream;
    return array;
}

c_sDevData *Cascade_Object::deDataStream(QByteArray &array)
{
    QDataStream out(&array, QIODevice::ReadOnly);
    out >> *mDataStream;
    return mCData;
}

void Cascade_Object::deDataStream(QByteArray &array, c_sDevData *dev)
{
    c_DataStream dataStream(dev);
    QDataStream out(&array, QIODevice::ReadOnly);
    out >> dataStream;
}

bool Cascade_Object::crcCheck(const QByteArray &array)
{
    bool ret = false;
    QByteArray ba = array.left(2);
    QByteArray end = array.right(2);
    ushort crc = (((uchar)end.at(0))<<8) + (uchar)end.at(1);
    ushort head = (((uchar)ba.at(0))<<8) + (uchar)ba.at(1);
    if((head == START_HEAD) && (END_CRC == crc)) ret = true;
    else qCritical() << "Error: Cascade crc" << cm::byteArrayToHexStr(array);

    return ret;
}
