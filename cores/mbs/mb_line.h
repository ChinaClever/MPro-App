#ifndef MB_LINE_H
#define MB_LINE_H

#include "mb_object.h"

class Mb_Line : public Mb_Object
{
    Q_OBJECT
public:
    explicit Mb_Line(QObject *parent = nullptr);

    void mbLineUpdate();
protected:

private:
    void upTgData();
    void upLineData();
    void upLineThreshold();

protected:
    template<typename T>
    void appendData(uchar size, const T v[], vshort &vs) {
        for(int i=0; i<size; ++i) vs << v[i];
    }

    void appendAlarm(const sAlarmUnit &unit, vshort &vs) {
        for(int i=0; i<unit.size; ++i) vs << unit.max[i] << unit.min[i];
    }

    void appendData2(uchar size, const uint v[], vshort &vs) {
        for(int i=0; i<size; ++i) vs << v[i] / 65536  << v[i] % 65536;
    }
};

#endif // MB_LINE_H
