#ifndef IPC_CORESERVER_H
#define IPC_CORESERVER_H

#include "ipc_logserver.h"

class IPC_CoreServer : public QObject
{
    Q_OBJECT
    explicit IPC_CoreServer(QObject *parent = nullptr);
public:
    static IPC_CoreServer *bulid(QObject *parent = nullptr);

protected slots:
    void initFunSlot();
};

#endif // IPC_CORESERVER_H
