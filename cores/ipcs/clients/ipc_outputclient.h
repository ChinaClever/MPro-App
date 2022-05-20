#ifndef IPC_OUTPUTCLIENT_H
#define IPC_OUTPUTCLIENT_H

#include "ipc_cfgclient.h"

class IPC_OutputClient : public IPC_ObjClient
{
    Q_OBJECT
    explicit IPC_OutputClient(QObject *parent = nullptr);
public:
    static IPC_OutputClient *bulid(QObject *parent = nullptr);
    bool ctrl(int addr, int id, int value);
    bool delaySet(int addr, int id, uchar value);
    bool swModeSet(int addr, int id, uchar value);
    bool opNameSet(int addr, int id, const QString &value);

private:
    bool msgSend(int addr, int id, int fc, const QVariant &msg);
    bool inputCheck(const QVariantList &values);
};

#endif // IPC_OUTPUTCLIENT_H
