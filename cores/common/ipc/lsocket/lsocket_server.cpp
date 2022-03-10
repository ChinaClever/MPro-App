#include "lsocket_server.h"

LSocket_Server::LSocket_Server(QObject *parent) : DBus_Receiver{parent}
{
    mSocketServer = nullptr;
}

LSocket_Server::~LSocket_Server()
{
    if(mSocketServer) mSocketServer->close();
}

bool LSocket_Server::lsCreateServer()
{
    bool ret = true;
    if(mSocketServer == nullptr) {
        mMonitor = LSocket_Monitor::bulid(this);
        mSocketServer = new QLocalServer(this);        
        connect(mSocketServer,SIGNAL(newConnection()),this,SLOT(ls_newConnectSlot()));
    } else ret = false;
    return ret;
}
bool LSocket_Server::lsListen(const QString &name)
{
    bool ret = false;
    if(name.size()) ret = lsCreateServer(); else return ret;
    if((name != key()) || ret) {
        mSocketServer->close();
        mSocketServer->removeServer(name);
        ret = mSocketServer->listen(name);
        if(ret) setKey(name); else qDebug() << mSocketServer->errorString();
    } else ret = true;

    return ret;
}

void LSocket_Server::ls_newConnectSlot()
{
    QLocalSocket *newsocket = mSocketServer->nextPendingConnection();  //获取连接上的客户端句柄
    connect(newsocket, SIGNAL(readyRead()), this, SLOT(ls_sRecvSlot())); //关联数据接收槽函数
    connect(newsocket, SIGNAL(disconnected()), this, SLOT(ls_disconnectSlot()));

    QString name = key();
    mMonitor->mOnLines.insert(name);
    mMonitor->mOffLines.remove(name);
    emit mMonitor->newConectSig(name);
}

void LSocket_Server::ls_sRecvSlot()
{
    QLocalSocket *local = static_cast<QLocalSocket *>(sender());
    if (local && local->isReadable()) {
        QByteArray rcv = local->readAll();
        rcv = lsRecv(rcv); if(rcv.size()) local->write(rcv);
    }
}

void LSocket_Server::ls_disconnectSlot()
{
    QString name = key();
    mMonitor->mOnLines.remove(name);
    mMonitor->mOffLines.insert(name);
    emit mMonitor->disconectSig(name);
}
