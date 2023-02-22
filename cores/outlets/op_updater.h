#ifndef OP_UPDATER_H
#define OP_UPDATER_H
#include "net_udp.h"
#include "op_object.h"
#include "cfg_file.h"

class OP_Updater : public OP_Object
{
    Q_OBJECT
public:
    explicit OP_Updater(QObject *parent = nullptr);
    bool ota_start(const QString &fn);

signals:
    void otaProgress(uchar addr, int v);
    void otaFinish(uchar addr, bool);
    void otaSig(int addr, const QString &message);

protected:
    bool ota_updates();

private:
    void ota_reboot();
    bool initOta(int id);
    void throwMessage(const QString &msg);
    bool ota_update(int addr, const QString &fn);
    bool ota_update(int addr, QByteArray &array);
    bool sendPacket(int addr, const QByteArray &array);

private slots:
    void onOtaFinish(uchar addr, bool ok);
    void onOtaProgress(uchar addr, int v);
    void onOtaSig(int addr, const QString &msg);

protected:
    bool isOta;
    QString mOtaFile;
    Net_Udp *mNet;
};

#endif // OP_UPDATER_H
