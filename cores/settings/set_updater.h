#ifndef SET_UPDATER_H
#define SET_UPDATER_H
#include "set_service.h"

class Set_Updater : public Set_Service
{
public:
    Set_Updater();

    QVariant otaStatus(int fc);
    bool ota_updater(int fc);
};

#endif // SET_UPDATER_H
