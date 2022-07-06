#ifndef INTEGR_RECEIVER_H
#define INTEGR_RECEIVER_H
#include "ws_server.h"
#include "integr_pushthread.h"

class Integr_Receiver : public Integr_PushThread
{
    Q_OBJECT
public:
    explicit Integr_Receiver(QObject *parent = nullptr);
    bool initWsServer(int port) {return mWs->initServer(port);}
    bool tcpListen(int port) {return mTcp->listen(port);}
    bool udpBind(int port) {return mUdp->bind(port);}

private slots:
    void recvSlot(const QByteArray &array);

private:
    Net_Udp *mUdp;
    WS_Server *mWs;
    Net_TcpServer *mTcp;
};

#endif // INTEGR_RECEIVER_H
