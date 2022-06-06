#ifndef APP_USB_H
#define APP_USB_H

#include <QtCore>

class App_Usb : public QThread
{
    Q_OBJECT
public:
    explicit App_Usb(QObject *parent = nullptr);

signals:

protected:
    void run();
};

#endif // APP_USB_H
