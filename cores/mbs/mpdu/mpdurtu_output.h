#ifndef MPDURTU_OUTPUT_H
#define MPDURTU_OUTPUT_H
#include "mpdurtu_loop.h"

class Mpdurtu_Output : public Mpdurtu_Loop
{
public:
    explicit Mpdurtu_Output(QObject *parent = nullptr);

protected:
    void output_Updatesw();
    void output_Update();
    void output_ctrl(ushort addr, ushort address, ushort value);
    void output_setting(ushort addr, ushort address, ushort value);

private:
    void output_BaseUpdate(uint v[], int startReg);
    void output_BaseUpdate2(uint v[], int startReg);
};

#endif // MPDURTU_OUTPUT_H
