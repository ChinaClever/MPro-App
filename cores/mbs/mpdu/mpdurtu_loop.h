#ifndef MPDURTU_LOOP_H
#define MPDURTU_LOOP_H

#include "mpdurtu_line.h"

class Mpdurtu_Loop : public Mpdurtu_Line
{
public:
    explicit Mpdurtu_Loop(QObject *parent = nullptr);
    void loop_Update();
    void loop_setting(ushort addr, ushort address, ushort value);

private:
    void loop_BaseUpdate(uint v[],int startReg);
    void loop_BaseUpdate2(uint v[],int startReg);

};

#endif // MPDURTU_LOOP_H
