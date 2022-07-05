#ifndef IPC_OBJCLIENT_H
#define IPC_OBJCLIENT_H

#include "domain_socketcli.h"

enum eLogs{
    eUserLog,
    eAlarmLog,
    eOpLog,
    eSysLog,
    eEleLog,
};

struct sIpcLog {
    sIpcLog() {id=fc=0; page=0;noe=30;}
    uchar id;
    uchar fc;
    uchar noe;
    ushort page;
};

class IPC_ObjClient : public QObject
{
    Q_OBJECT
public:
    explicit IPC_ObjClient(QObject *parent = nullptr);
    sDataPacket *dataPacket();

protected:
    bool sendSocket(const QVariantList &v);
    QVariant readSocket(const QVariantList &v, int msec=1000);
    virtual bool inputCheck(const QVariantList &){return true;}

private:
    Domain_SocketCli *mSocket;
};

#endif // IPC_OBJCLIENT_H
