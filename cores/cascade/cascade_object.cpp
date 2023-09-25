/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cascade_object.h"
#include "crc.h"

Cascade_Object::Cascade_Object(QObject *parent) : SerialPort{parent}
{
    mCData = new c_sDevData;
    mDataStream = new c_DataStream(mCData);
    memset((void *)mCData, 0, sizeof(c_sDevData));
}
/**
 * 设置级联对象的地址
 */
void Cascade_Object::setAddress()
{
    sParameter *p = &(cm::masterDev()->cfg.param);
    uchar addr = 1; if(p->devMode) addr = p->cascadeAddr;   /*非禁用级联模式，将级联地址赋值给addr*/
    mSelfAddr = addr;
}
/**
 * 将帧转换为字节数组
 */
QByteArray Cascade_Object::frameToArray(const c_sFrame &it)
{
    QByteArray array; QDataStream in(&array, QIODevice::WriteOnly);
    in << START_HEAD << it.dstAddr << mSelfAddr /*it.srcAddr*/ << it.fc << it.len;
    if(it.len) {in << it.data;} in << END_CRC;
    return array;   /*返回存储生成的数组*/
}
/**
 * 将数组转换为帧
 */
bool Cascade_Object::arrayToFrame(QDataStream &out, c_sFrame &it)
{
    ushort head, crc; bool ret = false; out >> head;
    if(head == START_HEAD) out >> it.dstAddr >> it.srcAddr >> it.fc >> it.len;
    if(it.len) {out >> it.data;} out >> crc;  //Crc::Cal(array)
    if(crc == END_CRC) ret = true;
    return ret; /*将数组转换为帧,成功则返回true*/
}
/**
 * 将字节数组解析为多个帧
 */
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

    return its;     /*返回存储解析得到的帧的its*/
}
/**
 * 根据条件筛选接收到的帧。将接收到的数组解析为多个帧，并根据给定的条件进行筛选。
 * 符合条件的帧添加到res中并返回
 */
QVector<c_sFrame> Cascade_Object::replyData(QByteArray &rcv, uchar addr, uchar fc)
{
    QVector<c_sFrame> res;
    QVector<c_sFrame> its = arrayToFrames(rcv); /*将字节数组解析为多个帧*/
    foreach(const auto &it, its) {
        if((it.srcAddr == addr)|| (addr == fc_mask)) {
            if(fc) if(fc != it.fc) continue;
            res << it;
        }
    }

    return res;     /*返回存储符合条件的帧*/
}

/**
 * 对给定的字节数组进行压缩
 */
QByteArray Cascade_Object::compress(QByteArray &value)
{
    QByteArray array = qCompress(value);
    //cout << value.size() << array.size();
    //Crc::Checksum(array);
    return array;
}
/**
 * 解压
 */
QByteArray Cascade_Object::uncompress(int addr, const QByteArray &value)
{
    static int cnt[DEV_NUM] = {0};
    QByteArray array = qUncompress(value);
    if((1 == cnt[addr]++) && array.isEmpty()) {
        sEventItem it; it.addr = addr;
        if(cm::cn()) {
            it.event_type = tr("级联通讯");
            it.event_content = tr("收到副机 %1 异常通讯数据").arg(addr);
        } else {
            it.event_type = "Cascading communication";
            it.event_content = tr("Abnormal communication data of auxiliary machine %1").arg(addr);
        } Log_Core::bulid()->append(it);
    } else if(array.size()) cnt[addr] = 0;
    if(array.isEmpty()) {cm::mdelay(3000); cout << addr;}
    return array;   /*返回解压缩后的数组*/
}


QVector<c_sFrame> Cascade_Object::readData(uchar fc, uchar addr)
{
    c_sFrame it; it.fc = fc; it.dstAddr = addr; it.len=0;
    QByteArray array = frameToArray(it);    /*将帧转换为字节数组*/
    array = transmit(compress(array));      /*将数组进行压缩处理*/
    if(array.size()) array = uncompress(addr, array);   /*压缩后的数据非空,则解压*/
    return arrayToFrames(array);    /*将字节数组解析为多个帧*/
}
/**
 * 将数据转换为帧并通过串口进行写入
 */
bool Cascade_Object::writeData(uchar fc, uchar addr, const QByteArray &value)
{
    c_sFrame it; it.fc = fc; it.dstAddr = addr;
    it.len = value.size(); it.data = value;
    QByteArray array = frameToArray(it);    /*将帧转换为字节数组*/
    return writeSerial(compress(array));    /*返回串口写入操作的结果*/
}

/**
 * 将数据转换为帧并进行传输
 */
QVector<c_sFrame> Cascade_Object::transData(uchar fc, uchar addr, const QByteArray &value)
{
    c_sFrame it; it.fc = fc; it.dstAddr = addr;
    it.len = value.size(); it.data = value;
    QByteArray array = frameToArray(it);    /*将帧转换为数组*/
    int t = 2654; //if(value.size() >= 5*1024) t *=4;
    array = transmit(compress(array), t);   /*将array数组先进行压缩，然后进行传输*/
    if(array.size()) array = uncompress(addr, array);   /*压缩后的数据非空,则解压*/
    return replyData(array, addr, fc);  /*根据条件筛选接收到的帧*/
}
/**
 * 将数据转换为帧并进行传输，并返回传输中最后一个帧的数据内容
 */
QByteArray Cascade_Object::tranData(uchar fc, uchar addr, const QByteArray &value)
{
    QByteArray array;
    QVector<c_sFrame> res = transData(fc, addr, value);
    if(res.size()) array = res.takeLast().data;
    return array;   /*返回存储最后一个帧数据内容*/
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
