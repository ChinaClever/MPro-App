#ifndef APP_START_H
#define APP_START_H

#include "app_timing.h"

class App_Start : public App_Timing
{
    Q_OBJECT
public:
    explicit App_Start(QObject *parent = nullptr);
    ~App_Start();

private:
    void compileTime();

private slots:
    void initFunSlot();
    void startThreadSlot();
};

#endif // APP_START_H
