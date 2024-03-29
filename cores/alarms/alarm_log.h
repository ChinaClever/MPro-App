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
    void currentAllAlarmClear() {for(int i=0; i<DEV_NUM; ++i) currentAlarmClear(i);}
    void currentAlarmClear(int addr){m_currentAlarm[addr].clear();}
    void appendAlarm(const sDataItem &index, uchar value);
    void appendSlaveOffline(int addr);
    void generateQRcode();

protected slots:
    void alarmSlot(const sDataItem &index, uchar value);

private:
    void resetAwtk();
    QString alarmRelay(uchar value);
    QString alarmStatus(uchar value);
    QString alarmSensor(uchar value);
    QString alarmContent(const sDataItem &index);
    sAlarmItem alarmItem(const sDataItem &index, uchar value);

private:
    QString m_currentAlarm[DEV_NUM];
};

#endif // ALARM_LOG_H
