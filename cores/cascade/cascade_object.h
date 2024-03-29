#ifndef CASCADE_OBJECT_H
#define CASCADE_OBJECT_H

#include "serialport.h"
#include "cascade_stream.h"

class Cascade_Object : public SerialPort
{
public:
    explicit Cascade_Object(QObject *parent = nullptr);
    enum {fc_readDev=0x51, fc_setting=0x11,  fc_setCfg,
          fc_otaStart=0x7B, fc_otaPack=0x7C, fc_otaEnd=0x7D,
          fc_mask=0xff};

protected:
    QVector<c_sFrame> readData(uchar fc, uchar addr);
    QVector<c_sFrame> replyData(QByteArray &rcv, uchar addr=0, uchar fc=0);
    QVector<c_sFrame> transData(uchar fc, uchar addr, const QByteArray &value);
    QByteArray tranData(uchar fc, uchar addr, const QByteArray &value);
    bool writeData(uchar fc, uchar addr, const QByteArray &value);
    uchar getAddress() {setAddress(); return mSelfAddr;}
    c_sDevData *deDataStream(QByteArray &array);
    c_sDevData *getDev() {return mCData;}
    QByteArray toDataStream();

private:
    QByteArray frameToArray(const c_sFrame &it);
    bool arrayToFrame(QDataStream &out, c_sFrame &it);
    QVector<c_sFrame> arrayToFrames(QByteArray &array);

    QByteArray compress(QByteArray &value);
    QByteArray uncompress(int addr, const QByteArray &value);
    void deDataStream(QByteArray &array, c_sDevData *dev);
    QByteArray toDataStream(c_sDevData *data);
    bool crcCheck(const QByteArray &array);
    void setAddress();

private:
    uchar mSelfAddr=1;
    c_sDevData *mCData;
    c_DataStream *mDataStream;
};

#endif // CASCADE_OBJECT_H
