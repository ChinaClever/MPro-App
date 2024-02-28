#ifndef MPDURTU_LINE_H
#define MPDURTU_LINE_H

#include "mpdurtu_object.h"

class Mpdurtu_Line : public Mpdurtu_Object
{
public:
    explicit Mpdurtu_Line(QObject *parent = nullptr);
    void line_Update();
    void line_setting(ushort addr, ushort address, ushort value);

private:
    void line_BaseUpdate(uint v[], int startReg);
    void line_BaseUpdate2(uint v[], int startReg);
};

#endif // MPDURTU_LINE_H
