#include "net_tcpserver.h"

Net_TcpServer::Net_TcpServer(QObject *parent)
    : QObject{parent}
{
    tcpServer = new QTcpServer(this);
}

void Net_TcpServer::initServer()
{

}