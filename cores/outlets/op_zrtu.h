#ifndef OP_ZRTU_H
#define OP_ZRTU_H

#include "op_zctrl.h"

class OP_ZRtu : public OP_ZCtrl
{
public:
    explicit OP_ZRtu(QObject *parent = nullptr);

protected:
    bool readData(int addr) override;

private:
    bool sendReadCmd(int addr, sOpIt *it);
    bool setEndisable(int addr, bool ret, uchar &v);
    bool recvPacket(const QByteArray &array, sOpIt *obj);
    void hardwareLog(int addr, const QByteArray &cmd);
    bool rtuLog(int addr, const QByteArray &array);
    void rtuThrowMessage(const QString &msg);

private:
    QByteArray m_array[DEV_NUM];
};

#endif // OP_ZRTU_H
