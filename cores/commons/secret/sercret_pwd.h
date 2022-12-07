#ifndef SERCRET_PWD_H
#define SERCRET_PWD_H
#include "sercret_sm.h"

class Sercret_Pwd : public Sercret_SM
{
public:
    Sercret_Pwd();
    QByteArray pwd_encode(const QByteArray &rawData);
};

#endif // SERCRET_PWD_H
