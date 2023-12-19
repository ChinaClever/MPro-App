#ifndef OP_PROCTRL_H
#define OP_PROCTRL_H
#include "op_artu.h"

class OP_ProCtrl : public OP_ARtu
{
public:
    explicit OP_ProCtrl(QObject *parent = nullptr);

private:
    void writeCtrlCmd(uchar *cmd, int k);
    void funSwitch(uchar *on, uchar *off, uchar all=0) override;
    void funClearEle(uchar *buf) override;
    void funDelay(uint *buf) override;
};

#endif // OP_PROCTRL_H
