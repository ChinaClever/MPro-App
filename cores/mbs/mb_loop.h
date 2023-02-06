#ifndef MB_LOOP_H
#define MB_LOOP_H

#include "mb_line.h"

class Mb_Loop : public Mb_Line
{
public:
    explicit Mb_Loop(QObject *parent = nullptr);


protected:
    void loop_update();
    void loop_setting(ushort addr, ushort value);

private:
    void loop_dataObj(vshort &vs, int id);
    void loop_dataUpdate();

    void loop_alarmObj(vshort &vs, int id);
    void loop_alarmUpdate();
    void loop_breakerUpdate();

    void loop_thresholdObj(const sAlarmUnit &unit, int id, vshort &vs);
    void loop_thresholdUpdate();
};

#endif // MB_LOOP_H
