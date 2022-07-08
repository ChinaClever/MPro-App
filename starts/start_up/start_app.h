#ifndef START_APP_H
#define START_APP_H

#include "app_usb.h"

class Start_App : public QObject
{
    Q_OBJECT
    explicit Start_App(QObject *parent = nullptr);
public:
    static Start_App *bulid(QObject *parent = nullptr);

protected:
    void initUsb();

private slots:
    void clearCacheSlot();

private:
    App_Usb *mUsb;
};

#endif // START_APP_H
