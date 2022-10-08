#ifndef APP_BUZZER_H
#define APP_BUZZER_H
#include "commons.h"
#include "cfg_obj.h"

class App_Buzzer : public QObject
{
    Q_OBJECT
public:
    explicit App_Buzzer(QObject *parent = nullptr);
    ~App_Buzzer();

private slots:
    void beep_offSlot();
    void beep_workDown();
    void beep_initFunSlot();
    void beep_onTimeoutDone();

private:
    int mBeep[2]={0,0};
    bool mBeepRun=false;
    QTimer *mBeepTimer;
};

#endif // APP_BUZZER_H
