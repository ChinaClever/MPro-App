#include "cascade_object.h"

Cascade_Object::Cascade_Object(QObject *parent) : SerialPort{parent}
{
    mCData = new c_sDevData;
    memset((void *)mCData, 0, sizeof(c_sDevData));
    mDataStream = new c_DataStream(mCData);
}

QByteArray Cascade_Object::replyData(uchar fc, QByteArray &rcv, uchar addr)
{
    ushort size = 0;
    QByteArray array;
    uchar _fc = 0, _addr = 0;

    QDataStream out(&rcv, QIODevice::ReadOnly);
    if((rcv.size()>4) && crcCheck(rcv)) {
        out >> _fc >> _addr >> size;
    }

    if((_fc == fc) && (_addr == addr)) {
        if(rcv.size() > 6) out >> array;
    } else {
        qCritical() << "Error: reply Data" << __FUNCTION__
                    << fc << addr << size << cm::byteArrayToHexStr(rcv);
    }

    return array;
}

QByteArray Cascade_Object::readData(uchar fc, uchar addr, ushort size)
{
    QByteArray array; QDataStream in(&array, QIODevice::WriteOnly);
    in << fc << addr << size << END_CRC; // Crc::Cal(array)
    array = transmit(array);
    return replyData(fc, array);
}

bool Cascade_Object::writeData(uchar fc, uchar addr, const QByteArray &value)
{
    QByteArray array; QDataStream in(&array, QIODevice::WriteOnly);
    in << fc << addr << (ushort)value.size() << value << END_CRC; // Crc::Cal(array)
    return writeSerial(array);
}

QByteArray Cascade_Object::transData(uchar fc, uchar addr, const QByteArray &value)
{
    QByteArray array; QDataStream in(&array, QIODevice::WriteOnly);
    in << fc << addr << (ushort)value.size() << value << END_CRC; // Crc::Cal(array);
    array = transmit(array);
    return replyData(fc, array);
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
    bool ret = false; QByteArray ba = array.right(2);
    //ushort crc = Crc::Cal((uchar *)array.data(), array.size()-2);
    ushort crc = (ba.at(0)<<8) + ba.at(1); if(END_CRC == crc) ret = true;
    else qCritical() << "Error: Cascade" << __FUNCTION__ << array.right(2).toShort()
                  << END_CRC << cm::byteArrayToHexStr(array);
    return ret;
}
