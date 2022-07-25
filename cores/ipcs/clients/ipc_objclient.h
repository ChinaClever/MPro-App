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

enum eLogFc {
    eLog_cnt=1,
    eLog_read,
    eLog_page,
    eLog_clear,
};

struct sLogFcIt {
    sLogFcIt() {type=fc=0; id=0;noe=30;}
    uchar type;
    uchar fc;
    uchar noe;
    uint id;
};

class IPC_ObjClient : public QObject
{
    Q_OBJECT
public:
    explicit IPC_ObjClient(QObject *parent = nullptr);
    sDataPacket *dataPacket();

protected:
    bool sendSocket(const QVariantList &v);
    QVariant readSocket(const QVariantList &v, int usec=3);
    virtual bool inputCheck(const QVariantList &){return true;}

private:
    Domain_SocketCli *mSocket;
};

#endif // IPC_OBJCLIENT_H
