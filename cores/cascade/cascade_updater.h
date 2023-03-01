#ifndef CASCADE_UPDATER_H
#define CASCADE_UPDATER_H
#include "cfg_file.h"
#include "net_udp.h"
#include "cascade_object.h"

class Cascade_Updater : public Cascade_Object
{
    Q_OBJECT
public:
    explicit Cascade_Updater(QObject *parent = nullptr);
    bool ota_update(int addr, const sOtaFile &it);
    void ota_updates();

public slots:
    void ota_start(const sOtaFile &it) {mIt=it;}

signals:
    void otaReplyFinishSig(const sOtaFile &it,bool);
    void otaSendSig(uchar addr, const QString &message);
    void otaProSig(uchar addr, int pro);

protected:
    bool otaReplyStart(const QByteArray &data);
    bool otaReplyFinish(const QByteArray &data);
    bool otaReplyPacket(const QByteArray &data);

private:
    void throwMessage(const QString &msg);
    bool otaSetFile(const QString &fn);
    bool otaSendInit(int addr, const sOtaFile &it);

    bool otaSendFinish(int addr, uint state);
    bool otaSendPacket(int addr, const QByteArray &array);
    bool otaSendData(uchar fn, int addr, const QByteArray &array);
    void otaRootfs(const QString &path);
    void otaReboot();

private slots:
    void rebootSlot(){system("reboot");}
    void otaRecvFinishSlot(const sOtaFile &it, bool ok);

protected:
    bool isOta;

private:
    int mCnt;
    int mSize;
    QFile *mFile;
    sOtaFile mIt;
    Net_Udp *mNet;
};

#endif // CASCADE_UPDATER_H
