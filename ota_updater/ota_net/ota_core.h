#ifndef OTA_CORE_H
#define OTA_CORE_H

#include "ota_net.h"

class Ota_Core : public Ota_Net
{
    Q_OBJECT
    explicit Ota_Core(QObject *parent = nullptr);
public:
    static Ota_Core *bulid(QObject *parent = nullptr);


};

#endif // OTA_CORE_H
