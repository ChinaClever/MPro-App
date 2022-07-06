#ifndef IPC_LOGSERVER_H
#define IPC_LOGSERVER_H

#include "ipc_objserver.h"

class IPC_LogServer : public IPC_ObjServer
{
    Q_OBJECT
public:
    explicit IPC_LogServer(QObject *parent = nullptr);

protected:
    QString logFun(const sIpcLog &it);

private:
    Sql_Statement *getSql(int id);
    QString readLog(int id, int page, int cnt);
};

#endif // IPC_LOGSERVER_H
