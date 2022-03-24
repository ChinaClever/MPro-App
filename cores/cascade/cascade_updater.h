#ifndef CASCADE_UPDATER_H
#define CASCADE_UPDATER_H
#include "file.h"
#include "cascade_object.h"

class Cascade_Updater : public Cascade_Object
{
    Q_OBJECT
public:
    explicit Cascade_Updater(QObject *parent = nullptr);
    bool ota_update(int addr, const sFileTrans &it);
    void ota_updates();

public slots:
    void ota_start(const sFileTrans &it) {mIt=it;}

signals:
    void otaReplyFinishSig(const sFileTrans &it,bool);
    void otaSendSig(uchar addr, const QString &message);

protected:
    bool otaReplyStart(const QByteArray &data);
    bool otaReplyFinish(const QByteArray &data);
    bool otaReplyPacket(const QByteArray &data);

private:
    bool otaSetFile(const QString &fn);
    bool otaSendInit(int addr, const sFileTrans &it);

    bool otaSendFinish(int addr, uint state);
    bool otaSendPacket(int addr, const QByteArray &array);
    bool otaSendData(uchar fn, int addr, const QByteArray &array);

protected:
    bool isOta;

private:
    int mSize;
    QFile *mFile;
    sFileTrans mIt;
};

#endif // CASCADE_UPDATER_H
