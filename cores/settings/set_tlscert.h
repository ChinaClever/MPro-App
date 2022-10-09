#ifndef SET_TLSCERT_H
#define SET_TLSCERT_H
#include "set_output.h"

struct sTlsCertIt
{
    sTlsCertIt() {}

};

class Set_TlsCert : public Set_Output
{
public:
    Set_TlsCert();

    QVariant getTlsCert(uchar fc);
    bool setTlsCert(uchar fc, const QVariant &v);

private:
    sTlsCertIt mTlsCert;
};

#endif // SET_TLSCERT_H
