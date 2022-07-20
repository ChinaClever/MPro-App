#ifndef INTEGR_RECEIVER_H
#define INTEGR_RECEIVER_H
#include "ws_server.h"
#include "integr_httpserver.h"

class Integr_Receiver : public Integr_PushThread
{
    Q_OBJECT
public:
    explicit Integr_Receiver(QObject *parent = nullptr);
    bool initWssServer(int port) {return mWss->initServer(port);}
    bool initWsServer(int port) {return mWs->initServer(port);}
    bool tcpListen(int port) {return mTcp->listen(port);}
    bool udpBind(int port) {return mUdp->bind(port);}
    void setRecvPort(int port);
    void startRecv(int en);

private slots:   
    void recvSlot(const QByteArray &array);

private:
    void initRecvFun();
    void closeServers();

private:
    Net_Udp *mUdp;
    WS_Server *mWs;
    WS_Server *mWss;
    Net_TcpServer *mTcp;
};

#endif // INTEGR_RECEIVER_H
