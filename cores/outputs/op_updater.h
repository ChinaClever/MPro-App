#ifndef OP_UPDATER_H
#define OP_UPDATER_H

#include "op_object.h"
#include "file.h"

class OP_Updater : public OP_Object
{
    Q_OBJECT
public:
    explicit OP_Updater(QObject *parent = nullptr);
    bool ota_start(const QString &fn);

signals:
    void otaPro(uchar addr, int v);
    void otaFinish(uchar addr, bool);
    void otaSig(int addr, const QString &message);

protected:
    bool ota_updates();

private:
    bool initOta(int id);
    bool ota_update(int addr, const QString &fn);
    bool ota_update(int addr, QByteArray &array);
    bool sendPacket(int addr, const QByteArray &array);

protected:
    bool isOta;
    QString mOtaFile;
};

#endif // OP_UPDATER_H
