#include "ws_client.h"

WS_Client::WS_Client(QObject *parent) : WS_Object{parent}
{
    isRecved = false;
    isConnected = false;
    mSocket = new QWebSocket();
}

bool WS_Client::open(const QString &url, int port)
{
    QString host = url + ":" + QString::number(port);
    initSocket(mSocket); mSocket->close(); mSocket->open(host);
    for(int i=0; i<1000; i+=10) if(isConnected)break; else mdelay(10);
    return isConnected;
}


