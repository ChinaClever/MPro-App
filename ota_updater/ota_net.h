#ifndef OTA_NET_H
#define OTA_NET_H

#include "ota_obj.h"

class Ota_Net : public Ota_Obj
{
    Q_OBJECT
public:
    explicit Ota_Net(QObject *parent = nullptr);
    void ota_updater(const sOtaFile &it, int bit, bool ok);
    void throwMessage(const QString &msg) {qDebug().noquote() << msg; mDtls->throwMessage(msg);}
    void ota_error(int error) {mLastError=error;}

private:
    bool coreRuning();
    QString unzip(const QString &fn);
    bool versionCheck(const QString &dir);
    void workDown(const QString &fn, int bit);
    int cmd_updater(const QString &fn, int bit);
    bool containsMAndPro(const QString& str);
    bool up_rootfs(const QString &path);
    bool rootfsExists(const QString &path);
    bool outletCheck(const QString &dir);

private slots:
    void rebootSlot();
    void startSlot(const QString &host);
    void finishSlot(const sOtaFile &it, bool ok);

private:
    int mLastError=0;
    Dtls_Recver *mDtls;

protected:
    sOtaUpdater *mOta;
};

#endif // OTA_NET_H
