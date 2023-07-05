#ifndef SET_TLSCERT_H
#define SET_TLSCERT_H
#include "set_output.h"

struct sTlsCertIt
{
    QString country; // 国家
    QString province; // 州或省
    QString city; // 地方性
    QString company; // 组织
    QString group; // 组织单位
    QString certname; // 通用名称
    QString mail; // 电子邮件地址
};

class Set_TlsCert : public Set_Output
{
public:
    Set_TlsCert();

    QVariant getTlsCert(uchar fc);
    bool setTlsCert(uchar fc, const QVariant &v);

private:
    bool createTlsCert();
    bool systemCmd(QString str);
    void tlsCertLog(int fc);

private:
    sTlsCertIt mTlsCert;
};

#endif // SET_TLSCERT_H
