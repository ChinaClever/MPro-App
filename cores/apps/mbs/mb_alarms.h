#ifndef MB_ALARMS_H
#define MB_ALARMS_H

#include "mb_settings.h"

class Mb_Alarms : public Mb_Settings
{
    Q_OBJECT
public:
    explicit Mb_Alarms(QObject *parent = nullptr);

signals:

};

#endif // MB_ALARMS_H
