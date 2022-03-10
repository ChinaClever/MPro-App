#ifndef IPC_LOGCLIENT_H
#define IPC_LOGCLIENT_H

#include "ipc_objclient.h"

class IPC_LogClient : public IPC_ObjClient
{
    Q_OBJECT
    explicit IPC_LogClient(QObject *parent = nullptr);
public:
    static IPC_LogClient *bulid(QObject *parent = nullptr);
    bool write(int id, const QStringList &value);
    QVariant read(int id, int page);

private:
    bool inputCheck(const QVariantList &values);
};

#endif // IPC_LOGCLIENT_H
