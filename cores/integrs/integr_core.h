#ifndef INTEGR_CORE_H
#define INTEGR_CORE_H

#include "integr_receiver.h"

class Integr_Core : public Integr_Receiver
{
    Q_OBJECT
    explicit Integr_Core(QObject *parent = nullptr);
public:
    static Integr_Core *bulid(QObject *parent = nullptr);

};

#endif // INTEGR_CORE_H