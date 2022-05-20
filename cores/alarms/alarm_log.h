#ifndef ALARM_LOG_H
#define ALARM_LOG_H

#include "alarm_Updater.h"

class Alarm_Log : public QObject
{
    Q_OBJECT
    explicit Alarm_Log(QObject *parent = nullptr);
public:
    static Alarm_Log *bulid(QObject *parent = nullptr);

protected slots:
    void alarmSlot(sDIndex index, uchar value);

private:
    QString alarmType(const sDIndex &index);
    QString alarmStatus(uchar value, QString &state);
    QString alarmContent(const sDIndex &index);

    QString alarmSensor(uchar value);
    QString alarmRelay(uchar value);
};

#endif // ALARM_LOG_H
