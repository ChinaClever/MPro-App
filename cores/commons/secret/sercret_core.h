#ifndef SERCRET_CORE_H
#define SERCRET_CORE_H
#include "sercret_pwd.h"

struct sSercretIt {
    sSercretIt(): type(0){}
    int type; // 加密模式
};

class Sercret_Core : public Sercret_Pwd
{
    Sercret_Core();
public:
    static Sercret_Core *bulid();
    static sSercretIt cfg;
    QByteArray encode(const QByteArray &raw);
    QByteArray decrypt(const QByteArray &raw);

    QByteArray base64_encode(const QByteArray &raw);
    QByteArray base64_decode(const QByteArray &raw);
};

#endif // SERCRET_CORE_H
