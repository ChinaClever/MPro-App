#ifndef MB_OUTPUT_H
#define MB_OUTPUT_H
#include "mb_line.h"

class Mb_Output : public Mb_Line
{
public:
    explicit Mb_Output(QObject *parent = nullptr);

protected:
    void output_update();

private:
    void output_dataUpdate();
    void output_alarmUpdate();
    void output_thresholdUpdate();
    void output_thresholdObj(const sAlarmUnit &unit, vshort &vs);
};

#endif // MB_OUTPUT_H
