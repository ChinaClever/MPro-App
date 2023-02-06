#ifndef MB_LINE_H
#define MB_LINE_H

#include "mb_object.h"

class Mb_Line : public Mb_Object
{
public:
    explicit Mb_Line(QObject *parent = nullptr);


protected:
    void line_update();
    void line_setting(ushort addr, ushort value);

private:
    void line_dataObj(vshort &vs, int id);
    void line_dataUpdate();

    void line_alarmObj(vshort &vs, int id);
    void line_alarmUpdate();

    void line_thresholdObj(const sAlarmUnit &unit, int id, vshort &vs);
    void line_thresholdUpdate();


};

#endif // MB_LINE_H
