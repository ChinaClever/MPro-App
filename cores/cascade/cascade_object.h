#ifndef CASCADE_OBJECT_H
#define CASCADE_OBJECT_H

#include "serialport.h"
#include "cascade_stream.h"




class Cascade_Object : public SerialPort, public Set_Relay
{
    Q_OBJECT
public:
    explicit Cascade_Object(QObject *parent = nullptr);
    enum {fc_readDev=0x51, fc_writeAlarm=0x11, fc_relayCtrl, fc_relaySet,
          fc_otaStart=0x7B, fc_otaPack=0x7C, fc_otaEnd=0x7D,
          fc_mask=0xff};

    uchar getAddress() {return mSelfAddr;}
    void setAddress(int addr){mSelfAddr=addr;}

protected:
    QVector<c_sFrame> readData(uchar fc, uchar addr);
    QVector<c_sFrame> replyData(QByteArray &rcv, uchar addr=0, uchar fc=0);
    QVector<c_sFrame> transData(uchar fc, uchar addr, const QByteArray &value);
    QByteArray tranData(uchar fc, uchar addr, const QByteArray &value);
    bool writeData(uchar fc, uchar addr, const QByteArray &value);

    c_sDevData *deDataStream(QByteArray &array);
    c_sDevData *getDev() {return mCData;}
    QByteArray toDataStream();

private:
    QByteArray frameToArray(const c_sFrame &it);
    bool arrayToFrame(QByteArray &array, c_sFrame &it);
    QVector<c_sFrame> arrayToFrames(QByteArray &array);

    void deDataStream(QByteArray &array, c_sDevData *dev);
    QByteArray toDataStream(c_sDevData *data);
    bool crcCheck(const QByteArray &array);

private:
    uchar mSelfAddr;
    c_sDevData *mCData;
    c_DataStream *mDataStream;
};

#endif // CASCADE_OBJECT_H
