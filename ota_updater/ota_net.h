#ifndef OTA_NET_H
#define OTA_NET_H

#include "ota_obj.h"

class Ota_Net : public Ota_Obj
{
    Q_OBJECT
public:
    explicit Ota_Net(QObject *parent = nullptr);
    void ota_updater(const sOtaFile &it, int bit, bool ok);

private:
    bool coreRuning();
    QString unzip(const QString &fn);
    bool versionCheck(const QString &dir);
    void workDown(const QString &fn, int bit);
    int cmd_updater(const QString &fn, int bit);
    bool up_rootfs(const QString &path);

private slots:
    void rebootSlot();
    void startSlot(const QString &host);
    void finishSlot(const sOtaFile &it, bool ok);
    void throwMessage(const QString &msg) {qDebug().noquote() << msg; mDtls->throwMessage(msg);}

private:
    Dtls_Recver *mDtls;

protected:
    sOtaUpdater *mOta;
};

#endif // OTA_NET_H
