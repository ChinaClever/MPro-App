#ifndef IPC_LOGCLIENT_H
#define IPC_LOGCLIENT_H

#include "ipc_echoclient.h"

class IPC_LogClient : public IPC_EchoClient
{
    Q_OBJECT
public:
    explicit IPC_LogClient(QObject *parent = nullptr);

    int log_counts(int id);
    bool log_clear(int id);
    QString log_read(int id, int page, int noe);
    QVariant log_fun(const sIpcLog &it);
private:
    QString log_get(const sIpcLog &unit);
    bool log_msgSend(const sIpcLog &msg);
};

#endif // IPC_LOGCLIENT_H
