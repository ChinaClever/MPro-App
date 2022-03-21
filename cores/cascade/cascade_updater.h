#ifndef CASCADE_UPDATER_H
#define CASCADE_UPDATER_H

#include "cascade_object.h"

class Cascade_Updater : public Cascade_Object
{
    Q_OBJECT
public:
    explicit Cascade_Updater(QObject *parent = nullptr);
    void ota_start(const QString &fn) {mOtaFile=fn;}
    bool ota_update(int addr, const QString &fn);
    void ota_updates(const QString &fn);
    void ota_updates();

signals:
    void otaReplyFinishSig(bool);
    void otaSendSig(uchar addr, const QString &message);

protected:
    bool otaReplyStart(const QByteArray &data);
    bool otaReplyFinish(const QByteArray &data);
    bool otaReplyPacket(const QByteArray &data);

private:
    bool otaSetFile(const QString &fn);
    bool otaSendInit(int addr, const QString &fn);
    bool otaSendFinish(int addr, uint state);
    bool otaSendPacket(int addr, const QByteArray &array);
    bool otaSendData(uchar fn, int addr, const QByteArray &array);

protected:
    bool isOta;

private:
    int mSize;
    QFile *mFile;
    QString mOtaFile;
};

#endif // CASCADE_UPDATER_H
