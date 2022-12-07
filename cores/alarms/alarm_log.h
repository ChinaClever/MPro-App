#ifndef ALARM_LOG_H
#define ALARM_LOG_H

#include "alarm_Updater.h"

class Alarm_Log : public QObject
{
    Q_OBJECT
    explicit Alarm_Log(QObject *parent = nullptr);
public:
    static Alarm_Log *bulid(QObject *parent = nullptr);
    QString alarmType(const sDataItem &index);

protected slots:
    void alarmSlot(const sDataItem &index, uchar value);

private:
    QString alarmStatus(uchar value);
    QString alarmContent(const sDataItem &index);

    QString alarmSensor(uchar value);
    QString alarmRelay(uchar value);
};

#endif // ALARM_LOG_H
