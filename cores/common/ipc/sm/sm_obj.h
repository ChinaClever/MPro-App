#ifndef SM_OBJ_H
#define SM_OBJ_H

#include "datapacket.h"
#include <QSharedMemory>
#define IPC_SHAREDMEMORY_KEY  "ipc_sharedmemory_pdu"

class SM_Obj : public QObject
{
    Q_OBJECT
public:
    explicit SM_Obj(QObject *parent = nullptr);
    static void *sharedMemory(int size);
    void *sharedMemory();

private:
    static void initShareMemory(QObject *parent = nullptr);
private:
    static QSharedMemory *mSm;
};


#endif // SM_OBJ_H
