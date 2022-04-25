#ifndef ALARM_UPDATER_H
#define ALARM_UPDATER_H

#include "alarm_object.h"

class Alarm_Updater : public QObject
{
    Q_OBJECT
    explicit Alarm_Updater(QObject *parent = nullptr);
public:
    static Alarm_Updater *bulid(QObject *parent = nullptr);

signals:
    void alarmSig(sAlarmIndex index, uchar value);

public slots:
    void run();

private:
    bool upRelayUnit(sAlarmIndex &index, sRelayUnit &it);

    bool upAlarmUnit(sAlarmIndex &index, sAlarmUnit &it);
    bool upObjData(sAlarmIndex &index, sObjData &it);
    bool upDevData(sAlarmIndex &index, sDevData *it);

    uchar upTgUnit(sAlarmIndex &index, sTgUnit &it);
    bool upTgObjData(sAlarmIndex &index, sTgObjData &it);
    bool upEnvData(sAlarmIndex &index, sEnvData &it);
    bool upDevAlarm(uchar addr);
};

#endif // ALARM_UPDATER_H
