#ifndef OP_ARTU_H
#define OP_ARTU_H

#include "op_objctrl.h"

class OP_ARtu : public OP_ObjCtrl
{
public:
    explicit OP_ARtu(QObject *parent = nullptr);

protected:
    bool loop_readData();
    bool loop_recvPacket(const QByteArray &array, sOpIt *it);
};

#endif // OP_ARTU_H
