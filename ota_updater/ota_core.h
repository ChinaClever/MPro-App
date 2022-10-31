#ifndef OTA_CORE_H
#define OTA_CORE_H

#include "ota_usb.h"

class Ota_Core : public Ota_Usb
{
    explicit Ota_Core(QObject *parent = nullptr);
public:
    static Ota_Core *bulid(QObject *parent = nullptr);
};

#endif // OTA_CORE_H
