#ifndef IPC_LOGCLIENT_H
#define IPC_LOGCLIENT_H

#include "ipc_objclient.h"

class IPC_LogClient : public IPC_ObjClient
{
    Q_OBJECT
    explicit IPC_LogClient(QObject *parent = nullptr);
public:
    static IPC_LogClient *bulid(QObject *parent = nullptr);
    bool write(eLogs id, const QStringList &value);
    QVariant read(eLogs id, int page);
    int countLog(eLogs id);
    void clearLog(eLogs id);

private:
    bool inputCheck(const QVariantList &values);
};

#endif // IPC_LOGCLIENT_H
