#ifndef OP_PRORTU_H
#define OP_PRORTU_H

#include "op_proctrl.h"

class OP_ProRtu : public OP_ProCtrl
{
public:
    explicit OP_ProRtu(QObject *parent = nullptr);

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

#endif // OP_PRORTU_H
