#ifndef APP_START_H
#define APP_START_H

#include "app_usb.h"

class App_Start : public QObject
{
    Q_OBJECT
    explicit App_Start(QObject *parent = nullptr);
public:
    static App_Start *bulid(QObject *parent = nullptr);

private:
    void initUsb();
    void initSystem();

private slots:
    void initFunSlot();
    void startThreadSlot();
    void clearCacheSlot();

private:
    App_Usb *mUsb;
};

#endif // APP_START_H
