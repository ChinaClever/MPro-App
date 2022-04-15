#ifndef IPC_ALARMCLIENT_H
#define IPC_ALARMCLIENT_H

#include "ipc_delayclient.h"

class IPC_AlarmClient : public IPC_ObjClient
{
    Q_OBJECT
    explicit IPC_AlarmClient(QObject *parent = nullptr);
public:
    static IPC_AlarmClient *bulid(QObject *parent = nullptr);
    bool setAlarm(const sSetAlarmUnit &unit);

signals:

};

#endif // IPC_ALARMCLIENT_H
