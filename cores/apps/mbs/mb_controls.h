#ifndef MB_CONTROLS_H
#define MB_CONTROLS_H

#include "mb_alarms.h"

class Mb_Controls : public Mb_Alarms
{
    Q_OBJECT
public:
    explicit Mb_Controls(QObject *parent = nullptr);

signals:

};

#endif // MB_CONTROLS_H
