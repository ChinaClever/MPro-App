#ifndef SSDP_OBJ_H
#define SSDP_OBJ_H

#include "net_udp.h"
#include "commons.h"

#define START_HEAD ((ushort)0xC5C5)
#define END_CRC ((ushort)0x5C5C)

struct sSdpIt{
    sSdpIt():version(1){}
    int fc;
    int version;
    QString room;
    QString ip;
    QString target;
    QByteArray data;
    ushort crc;
};

class Ssdp_Obj : public QObject
{
    Q_OBJECT
public:
    explicit Ssdp_Obj(QObject *parent = nullptr);    
    ~Ssdp_Obj();
    bool ssdpSend(const sSdpIt &it);
    bool udpSend(const sSdpIt &it);
    bool ssdpBind(bool en=true);
    void ssdpClose();

signals:
    void recvSig(const sSdpIt &it);

protected:
    bool udpBind();

private:
    void recvMsg(const QByteArray &array);
    QByteArray toArray(const sSdpIt &it);
    bool toItem(const QByteArray &array, sSdpIt &it);

protected slots:
    virtual void readMsgSlot();
    void reSsdpBindSlot() {ssdpBind();}
    void recvUdpSlot(const QByteArray &array){recvMsg(array);}
    virtual bool rplySearchTarget(const sSdpIt &){return false;}

private:
    int mPort;
    Net_Udp *mUdp;
    QHostAddress mHost;
    QUdpSocket *mSocket;
    QHostAddress mAddress;
    bool isOpen=false;
};

#endif // SSDP_OBJ_H
