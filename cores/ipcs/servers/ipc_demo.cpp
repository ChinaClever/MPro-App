#include "ipc_demo.h"

IPC_Demo::IPC_Demo(QObject *parent) : IPC_ObjServer{parent}
{
}

IPC_Demo *IPC_Demo::bulid(QObject *parent)
{
    static IPC_Demo *sington = nullptr;
    if(!sington) {
        sington = new IPC_Demo(parent);
        sington->initFunction("demo");
    }
    return sington;
}

QList<const char *> IPC_Demo::busRecvMethods()
{
    QList<const char *> res;
    res << SLOT(dbus_recv_slot(int, int));
    return res;
}

void IPC_Demo::dbus_recv_slot(const int &v, const int &i)
{
    qDebug() << "dbus_recv_slot" << v << i;
}

QStringList IPC_Demo::dbus_reply_slot(const int &i, const QString &v)
{
    qDebug() << "dbus_reply_slot"  << i << v;

    QStringList res {v};
    res << "123";

    return res;
}

QByteArray IPC_Demo::lsRecv(const QByteArray &v)
{
    qDebug() << "lsocket_recv:" << v;
    return v;
}

void ipc_demo(QObject *p)
{
    DBus_Object::registerBusService();
    IPC_Demo *demo = IPC_Demo::bulid(p);

    QString key = "demo";
    DBus_Sender sender(key, p);
    QVariantList i{105, 204};
    sender.sendBus(i);

    DBus_Call call(key, p);
    QVariantList v{5, "lzy"};
    QVariantList res = call.callBus(v);
    foreach(auto &i,res) qDebug() << i.typeName() << i;

    LSocket_Client *clientSocket = new LSocket_Client(key, p);
    qDebug() << "LSocket Client A" << clientSocket->writeLsc("123", key);
    qDebug() << "LSocket Client B" << clientSocket->transLsc("456");
    qDebug() << "LSocket Monitor C" << LSocket_Monitor::bulid(p)->onLines();

    demo->dataPacket()->lzy = 99;

    SM_Obj readSm;
    sDataPacket *rsp = (sDataPacket *) readSm.sharedMemory();
    qDebug() << "IPC Shared Memory" << rsp->lzy;
}
