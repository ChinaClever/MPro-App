#ifndef OP_ARTU_H
#define OP_ARTU_H
#include "op_sernum.h"

class OP_ARtu : public Op_SerialNumber
{
public:
    explicit OP_ARtu(QObject *parent = nullptr);

protected:
    bool loop_readData();

private:
    bool loop_setEndisable(bool ret, uchar &v);
    bool loop_recvPacket(const QByteArray &array, sOpIt *it);
};

#endif // OP_ARTU_H
