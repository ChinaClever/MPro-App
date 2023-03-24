#ifndef ALARM_LOG_H
#define ALARM_LOG_H
#include "log_core.h"
#include "alarm_Updater.h"

class Alarm_Log : public QObject
{
    Q_OBJECT
    explicit Alarm_Log(QObject *parent = nullptr);
public:
    static Alarm_Log *bulid(QObject *parent = nullptr);
    QString alarmType(const sDataItem &index);

    QString getCurrentAlarm(int addr);
    void currentAlarmClear(int addr){m_currentAlarm[addr].clear();}
    void appendAlarm(const sDataItem &index, uchar value);
    void appendSlaveOffline(int addr);
    void generateQRcode();

protected slots:
    void alarmSlot(const sDataItem &index, uchar value);

private:
    QString alarmStatus(uchar value);
    QString alarmContent(const sDataItem &index);
    QString alarmSensor(uchar value);
    QString alarmRelay(uchar value);
    sAlarmItem alarmItem(const sDataItem &index, uchar value);

private:
    QString m_currentAlarm[DEV_NUM];
};

#endif // ALARM_LOG_H
