#ifndef OP_OBJCTRL_H
#define OP_OBJCTRL_H

#include "op_objrtu.h"

class OP_ObjCtrl : public OP_ObjRtu
{
    Q_OBJECT
public:
    explicit OP_ObjCtrl(QObject *parent = nullptr);

    void clearEle(int id);
    void relayCtrl(int id, int on);
    void setDelay(int id, uchar sec);

protected:
    virtual void openAllSwitch();
    virtual void closeAllSwitch();

    virtual void openSwitch(int id);
    virtual void closeSwitch(int id);

    virtual void closeOtherSwitch(int id);
    virtual void openOnlySwitch(int id);

    virtual void clearAllEle();
    virtual void setClearEle(int id);

    virtual void setAllDelay(uchar sec);
    virtual void setOutputDelay(int id, uchar sec);
    virtual void setBitControl(int id, uchar *buf);

    virtual void funDelay(uchar *buf)=0;
    virtual void funClearEle(uchar *buf)=0;
    virtual void funSwitch(uchar *on, uchar *off)=0;
};

#endif // OP_OBJCTRL_H
