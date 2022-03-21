#ifndef LSOCKET_SERVER_H
#define LSOCKET_SERVER_H
#include "lsocket_monitor.h"

class LSocket_Server : public DBus_Receiver
{
    Q_OBJECT
public:
    explicit LSocket_Server(QObject *parent = nullptr);
    virtual QByteArray lsRecv(const QByteArray &) = 0;

    bool lsListen(){return lsListen(key());}
    bool lsListen(const QString &name);
    ~LSocket_Server();

signals:
    void ls_newConnectSig();

private:
    bool lsCreateServer();

private slots:
    void ls_sRecvSlot();
    void ls_newConnectSlot();
    void ls_disconnectSlot();

private:    
    QLocalServer *mSocketServer;
    LSocket_Monitor *mMonitor;
};

#endif // LSOCKET_SERVER_H
