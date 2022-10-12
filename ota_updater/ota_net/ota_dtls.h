#ifndef OTA_DTLS_H
#define OTA_DTLS_H

#include "ota_obj.h"

class Ota_Dtls : public Ota_Obj
{
    Q_OBJECT
public:
    explicit Ota_Dtls(QObject *parent = nullptr);

signals:

private:
    Dtls_Recver *mDtls;
};

#endif // OTA_DTLS_H
