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

protected:
    QByteArray replyData(uchar fc, QByteArray &rcv, uchar addr=0);
    QByteArray readData(uchar fc, uchar addr, ushort size);
    bool writeData(uchar fc, uchar addr, const QByteArray &value);
    QByteArray transData(uchar fc, uchar addr, const QByteArray &value);

    QByteArray toDataStream();
    QByteArray toDataStream(c_sDevData *data);

    c_sDevData *deDataStream(QByteArray &array);
    void deDataStream(QByteArray &array, c_sDevData *dev);
    c_sDevData *getDev() {return mCData;}
    bool crcCheck(const QByteArray &array);

private:
    c_sDevData *mCData;
    c_DataStream *mDataStream;
};

#endif // CASCADE_OBJECT_H
