#include "ipc_alarmserver.h"

IPC_AlarmServer::IPC_AlarmServer(QObject *parent)
    : IPC_ObjServer{parent}
{

}

IPC_AlarmServer *IPC_AlarmServer::bulid(QObject *parent)
{
    static IPC_AlarmServer *sington = nullptr;
    if(!sington) {
        sington = new IPC_AlarmServer(parent);
        sington->initFunction(IPC_KEY_ALARM);
    }
    return sington;
}

QList<const char *> IPC_AlarmServer::busRecvMethods()
{
    QList<const char *> res;
    res << SLOT(dbus_recv_slot(QByteArray));
    return res;
}

void IPC_AlarmServer::dbus_recv_slot(const QByteArray &array)
{
    sSetAlarmUnit unit = cm::toStruct<sSetAlarmUnit>(array);
    Set_Core *set = Set_Core::bulid();
    set->setAlarm(unit);
}
