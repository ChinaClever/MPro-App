#ifndef INTEGR_RECEIVER_H
#define INTEGR_RECEIVER_H
#include "ws_server.h"
#include "integr_httpserver.h"

class Integr_Receiver : public Integr_PushThread
{
    Q_OBJECT
public:
    explicit Integr_Receiver(QObject *parent = nullptr);
    void initRecvFun();
    ~Integr_Receiver();

private slots:
    void workDown();
    void recvSlot(const QByteArray &array);
    void recvUdpSlot(const QByteArray &array);

private:
    void closeServers();

private:
    Net_Udp *mUdp;
    WS_Server *mWs;
    WS_Server *mWss;
    Net_TcpServer *mTcp;
    Mqtt_Client *mMqtt;
    QRabbitMQ *mAmqp;
    QByteArrayList mList;
    bool isRun=true;
};

#endif // INTEGR_RECEIVER_H
