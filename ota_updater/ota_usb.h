#ifndef OTA_USB_H
#define OTA_USB_H

#include "ota_net.h"

class Ota_Usb : public Ota_Net
{
    Q_OBJECT
public:
    explicit Ota_Usb(QObject *parent = nullptr);
    void usb_run();

signals:
    void usbSig();

private slots:
    void usb_initSlot();
    void usb_netSLot();
    void usb_otaSlot();

private:
    bool isUsbRun=false;
};

#endif // OTA_USB_H
