#ifndef SET_UPDATER_H
#define SET_UPDATER_H
#include "set_service.h"

class Set_Updater : public Set_Service
{
public:
    Set_Updater();

    QVariant otaStatus(int type, int fc);
    int ota_updater(int fc, const QVariant &v);

private:
    void ota_log(int fc);
    bool ota_outlet(int fc);
    bool ota_cascade(const QString &fn);
};

#endif // SET_UPDATER_H
