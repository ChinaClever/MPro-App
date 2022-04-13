#include "cascade_object.h"

Cascade_Object::Cascade_Object(QObject *parent) : SerialPort{parent}
{
    setAddress(0);
    mCData = new c_sDevData;
    memset((void *)mCData, 0, sizeof(c_sDevData));
    mDataStream = new c_DataStream(mCData);
}

QByteArray Cascade_Object::frameToArray(const c_sFrame &it)
{
    QByteArray array; QDataStream in(&array, QIODevice::WriteOnly);
    in << START_HEAD << it.dstAddr << mSelfAddr /*it.srcAddr*/ << it.fc << it.len;
    if(it.len) {in << it.data;} in << END_CRC;
    return array;
}

bool Cascade_Object::arrayToFrame(QByteArray &array, c_sFrame &it)
{
    ushort head, crc; bool ret = false;
    QDataStream out(&array, QIODevice::ReadOnly);
    if((array.size()>6) && crcCheck(array)) {
        out >> head >> it.dstAddr >> it.srcAddr >> it.fc >> it.len;
        if(it.len) {out >> it.data;} out >> crc; // Crc::Cal(array)
        array.remove(0, it.len + 9); ret = true;
    } else qCritical() << "Error:" << __FUNCTION__ << cm::byteArrayToHexStr(array);
    return ret;
}

QVector<c_sFrame> Cascade_Object::arrayToFrames(QByteArray &array)
{
    QVector<c_sFrame> its;
    for(int i=0; i<array.size(); ++i) {
        c_sFrame it; bool ret = arrayToFrame(array, it);
        if(ret) its << it; else array.remove(0, 1);
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

    return its;
}

QVector<c_sFrame> Cascade_Object::readData(uchar fc, uchar addr)
{
    c_sFrame it; it.fc = fc; it.dstAddr = addr; it.len=0;
    QByteArray array = frameToArray(it); array = transmit(array);
    return arrayToFrames(array);
}

bool Cascade_Object::writeData(uchar fc, uchar addr, const QByteArray &value)
{
    c_sFrame it; it.fc = fc; it.dstAddr = addr;
    it.len=value.size(); it.data = value;
    QByteArray array = frameToArray(it);
    return writeSerial(array);
}

QVector<c_sFrame> Cascade_Object::transData(uchar fc, uchar addr, const QByteArray &value)
{
    c_sFrame it; it.fc = fc; it.dstAddr = addr;
    it.len=value.size(); it.data = value;
    QByteArray array = frameToArray(it);
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
    ushort head = (ba.at(0)<<8) + ba.at(1);
    if(head == START_HEAD) {
        ushort len = ((uchar)array.at(5))*256 + (uchar)array.at(6);
        QByteArray end = array.mid(7+len, 2);
        ushort crc = (end.at(0)<<8) + end.at(1);
        if(END_CRC == crc) ret = true; else qCritical() << "Error: Cascade end crc";
    } else qCritical() << "Error: Cascade head crc";

    return ret;
}
