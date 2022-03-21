#ifndef OP_UPDATER_H
#define OP_UPDATER_H

#include "op_object.h"

class OP_Updater : public OP_Object
{
    Q_OBJECT
public:
    explicit OP_Updater(QObject *parent = nullptr);
    bool ota_update(int addr, const QString &fn);
    bool ota_update(int addr, QByteArray &array);

signals:
    void otaSig(const QString &message);

private:
    bool initOta(int id);
    bool sendPacket(int addr, const QByteArray &array);

protected:
    bool isOta;
};

#endif // OP_UPDATER_H
