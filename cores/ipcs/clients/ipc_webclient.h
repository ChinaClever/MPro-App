#ifndef IPC_WEBCLIENT_H
#define IPC_WEBCLIENT_H

#include "ipc_outputclient.h"

class IPC_WebClient : public IPC_OutputClient
{
    Q_OBJECT
    explicit IPC_WebClient(QObject *parent = nullptr);
public:
    static IPC_WebClient *bulid(QObject *parent = nullptr);

    bool devNameSet(int addr, const QString &value, int id=6);

};

#endif // IPC_WEBCLIENT_H
