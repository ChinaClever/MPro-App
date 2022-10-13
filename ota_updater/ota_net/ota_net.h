#ifndef OTA_NET_H
#define OTA_NET_H

#include "ota_obj.h"

class Ota_Net : public Ota_Obj
{
    Q_OBJECT
public:
    explicit Ota_Net(QObject *parent = nullptr);

private:
    void workDown();
    bool coreRuning();
    void cmd_updater(bool ok);
    QString unzip(const QString &fn);

private slots:
    void startSlot(const QString &host);
    void finishSlot(const sOtaFile &it, bool ok);
    void throwMessage(const QString &msg) {mDtls->throwMessage(msg);}

private:
    sOtaUpdater *mOta;
    Dtls_Recver *mDtls;
};

#endif // OTA_NET_H
