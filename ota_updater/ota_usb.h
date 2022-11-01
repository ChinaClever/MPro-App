#ifndef OTA_USB_H
#define OTA_USB_H

#include "ota_net.h"

class Ota_Usb : public Ota_Net
{
    Q_OBJECT
public:
    explicit Ota_Usb(QObject *parent = nullptr);

signals:

};

#endif // OTA_USB_H
