#ifndef OTA_OBJ_H
#define OTA_OBJ_H

#include "dtls_sender.h"
#include "otassdp.h"

class Ota_Obj : public QObject
{
    Q_OBJECT
public:
    explicit Ota_Obj(QObject *parent = nullptr);
    virtual void web_workDown()=0;

protected:
    void onStartSsdp();
    bool sign_verify(const sOtaFile &it);
    void sign_init();

private slots:
    void runing_initFunSlot();
    void runing_onTimeoutDone();

private:
    QTimer *mTimer;
    OtaSsdp *mSsdp=nullptr;
};

#endif // OTA_OBJ_H
