#ifndef ALARM_UPDATER_H
#define ALARM_UPDATER_H

#include "alarm_object.h"

class Alarm_Updater : public QObject
{
    Q_OBJECT
    explicit Alarm_Updater(QObject *parent = nullptr);
public:
    static Alarm_Updater *bulid(QObject *parent = nullptr);
    bool upDevAlarm(uchar addr);

signals:
    void alarmSig(const sDataItem &index, uchar value);
    void runSig();

public slots:
    void run();

private:
    bool upLoopVol(const sDataItem &index);
    bool upCorrectData(int i, sAlarmUnit &it);
    bool upRelayUnit(sDataItem &index, sRelayUnit &it);
    bool upAlarmItem(sDataItem &index, int i, sAlarmUnit &it);
    void upPeakValue(sDataItem &index, int i, sAlarmUnit &it);

    void upEleHda(sDataItem &index, sObjData &it);
    bool upAlarmUnit(sDataItem &index, sAlarmUnit &it);
    bool upObjData(sDataItem &index, sObjData &it);
    bool upDevData(sDataItem &index, sDevData *it);

    uchar upTgUnit(sDataItem &index, sTgUnit &it);
    bool upTgObjData(sDataItem &index, sTgObjData &it);
    bool upEnvData(sDataItem &index, sEnvData &it);

    bool upSensorStatus(sDataItem &index, uint *ptr, int id=0);
    bool upSensors(sDataItem &index, sEnvData &it);
    bool upCabData(sDataItem &index, sDevData *it);

private:
    int mCrAlarm=0;
};

#endif // ALARM_UPDATER_H
