#ifndef MB_OUTPUT_H
#define MB_OUTPUT_H
#include "mb_loop.h"

class Mb_Output : public Mb_Loop
{
public:
    explicit Mb_Output(QObject *parent = nullptr);

protected:
    void output_update();
    void output_clearEle(uchar addr, uchar id);
    void output_upDelay(uchar addr, uchar id, ushort value);
    void output_ctrl(ushort addr, ushort address, ushort value);
    void output_setting(ushort addr, ushort address, ushort value);
    void output_dualCtrl(ushort addr, ushort address, ushort value);

private:
    void output_dataObj(vshort &vs, int id);
    void output_objUpdate();

    void output_dataUpdate();
    void output_alarmUpdate();
    void output_relayUpdate();
    void output_thresholdUpdate();
    void output_thresholdObj(const sAlarmUnit &unit, vshort &vs);
};

#endif // MB_OUTPUT_H
