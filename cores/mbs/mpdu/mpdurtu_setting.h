#ifndef MPDURTU_SETTING_H
#define MPDURTU_SETTING_H

#include "mpdurtu_env.h"

class Mpdurtu_Setting : public Mpdurtu_Env
{
public:
    explicit Mpdurtu_Setting(QObject *parent = nullptr);

protected:
    void mbSetUpdate();
private slots:
    //void rcvDataSlot(int address, vshort values);
    void registerRecvSlot(int address,ushort value);

};

#endif // MPDURTU_SETTING_H
