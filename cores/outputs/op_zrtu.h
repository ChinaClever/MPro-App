#ifndef OP_ZRTU_H
#define OP_ZRTU_H

#include "op_zctrl.h"

class OP_ZRtu : public OP_ZCtrl
{
    Q_OBJECT
    explicit OP_ZRtu(QObject *parent = nullptr);
public:
    static OP_ZRtu *bulid(QObject *parent = nullptr);
    bool readData(int addr, sOpIt &it);

private:
    bool sendReadCmd(int addr, sOpIt &it);
    bool recvPacket(const QByteArray &array, sOpIt &it);
};

#endif // OP_ZRTU_H
