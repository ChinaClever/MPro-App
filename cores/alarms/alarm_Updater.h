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
    void alarmSig(sDIndex index, uchar value);

public slots:
    void run();

private:
    bool upRelayUnit(sDIndex &index, sRelayUnit &it);

    bool upAlarmUnit(sDIndex &index, sAlarmUnit &it);
    bool upObjData(sDIndex &index, sObjData &it);
    bool upDevData(sDIndex &index, sDevData *it);

    uchar upTgUnit(sDIndex &index, sTgUnit &it);
    bool upTgObjData(sDIndex &index, sTgObjData &it);
    bool upEnvData(sDIndex &index, sEnvData &it);

    bool upSensorStatus(sDIndex &index, uint *ptr, int id=0);
    bool upSensors(sDIndex &index, sEnvData &it);
    bool upDevAlarm(uchar addr);
};

#endif // ALARM_UPDATER_H
