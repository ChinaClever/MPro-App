#ifndef OP_OBJCTRL_H
#define OP_OBJCTRL_H

#include "op_objrtu.h"

class OP_ObjCtrl : public OP_ObjRtu
{
    Q_OBJECT
public:
    explicit OP_ObjCtrl(QObject *parent = nullptr);

    void clearEle(int id);
    void clearEle(int start, int end);
    void relayCtrl(int id, int on);
    void setDelay(int id, uchar sec);
    void orderCtrl(int on, uchar all=1);
    void relaysCtrl(int start, int end, int v);
    void relaysCtrl(const QList<int> &ids, int v);

protected:    
    virtual void funDelay(uint *buf)=0;
    virtual void funClearEle(uchar *buf)=0;
    virtual void funSwitch(uchar *on, uchar *off, uchar all=0)=0;

private slots:
    void relayResetSlot();

private:
    void openAllSwitch(uchar all);
    void closeAllSwitch(uchar all);

    void openSwitch(int id);
    void closeSwitch(int id);

    void closeOtherSwitch(int id);
    void openOnlySwitch(int id);

    void clearAllEle();
    void setClearEle(int id);

    void setAllDelay(uchar sec);
    void setOutputDelay(int id, uchar sec);
    void setBitControl(int id, uchar *buf);

private:
    QList<int> mList;
};

#endif // OP_OBJCTRL_H
