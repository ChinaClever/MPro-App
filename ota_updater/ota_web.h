#ifndef OTA_WEB_H
#define OTA_WEB_H

#include "ota_usb.h"

class Ota_Web : public Ota_Usb
{
    Q_OBJECT
public:
    explicit Ota_Web(QObject *parent = nullptr);

private:
    void web_workDown();
    bool web_readRsaSig(sOtaFile &it);
};

#endif // OTA_WEB_H
