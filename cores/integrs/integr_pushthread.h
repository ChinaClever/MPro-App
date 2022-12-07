#ifndef INTEGR_PUSHTHREAD_H
#define INTEGR_PUSHTHREAD_H
#include "mqtt_client.h"
#include "net_tcpserver.h"
#include "integr_jsonrecv.h"
#include "qrabbitmq.h"
#define INTEGR_UDP_SIZE  4

struct sPushUdpCfg{
    int en = 0;
    QString host;
    int port=1124;
    int sec = 5;
};

struct sPushHttpCfg{
    int en = 0; // 0 禁此 1 post 2 put
    QString url;
    int timeout=1;
    int sec = 5;
    int enServer =0;
    int port=3066;
};

struct sPushCfg{
    sPushUdpCfg udp[INTEGR_UDP_SIZE];
    sPushHttpCfg http;
    int recvPort=3096;
    int recvEn = 0; // 0 禁此 1 UDP 2 TCP 3
};


class Integr_PushThread : public QObject
{
    Q_OBJECT
public:
    explicit Integr_PushThread(QObject *parent = nullptr);
    ~Integr_PushThread() {isRun = false;}
    static sPushCfg pushCfg;

signals:
    void pushSig();

public slots:
    void run();
    void onPushSlot();

private:
    void workDown();
    bool checkPush();
    void delay(int msec);
    void udpPush(const QByteArray &array);
    void httpPush(const QByteArray &array);
    void mqttPush(const QByteArray &array);
    void amqpPush(const QByteArray &array);

private:    
    uint mCnt = 0;
    QByteArray mArray;
    bool isRun = true;
    Net_Udp *mUdp = nullptr;
    Integr_JsonBuild *mJson;
    sPushCfg *mCfg = &pushCfg;
};

#endif // INTEGR_PUSHTHREAD_H
