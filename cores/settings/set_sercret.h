#ifndef SET_SERCRET_H
#define SET_SERCRET_H
#include "set_output.h"

class Set_Sercret : public Set_Output
{
public:
    Set_Sercret();

    QVariant getSercret(uchar fc);
    bool setSercret(uchar fc, const QVariant &v);

    QVariant getTlsCert(uchar fc);
    bool setTlsCert(uchar fc, const QVariant &v);

    QVariant whiteListCfg(int fc);
    bool setWhiteList(uchar fc, const QVariant &v);
};

#endif // SET_SERCRET_H
