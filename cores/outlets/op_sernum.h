#ifndef OP_SERNUM_H
#define OP_SERNUM_H

#include "op_objctrl.h"

struct sOpSnItem
{
    sOpSnItem() {}
    uchar devType[4];
    uchar date[4];
    ushort num;
    uchar pc;
    uchar exor;
};


class Op_SerialNumber : public OP_ObjCtrl
{
public:
    explicit Op_SerialNumber(QObject *parent = nullptr);
    QString sn_get(uchar addr) {return m_sn[addr];}
    bool sn_read(uchar addr);

private:
    int sn_initReadCmd(uchar addr, uchar *sn);
    bool sn_analy(uchar *sn, int len);
    bool sn_check(uchar *sn, int len);
    void sn_toStr(uchar addr);

private:
    QString m_sn[DEV_NUM];
    sOpSnItem mSnItem;
};

#endif // OP_SERNUM_H
