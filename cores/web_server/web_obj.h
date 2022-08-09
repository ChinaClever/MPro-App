#ifndef WEB_OBJ_H
#define WEB_OBJ_H
#include <QtCore>
#include "mongoose.h"
#include "set_core.h"

class Web_Obj
{
public:
    Web_Obj();
    double getNumber(mg_str &r, int id);
    QString getString(mg_str &r, int id);
    QVector<double> getNumbers(mg_str &r, int num);

    QString getCfg(uchar addr, uchar fc, uchar id);
    bool setCfg(uint addr, uchar fc, uchar type, const QVariant &value);
    int getValue(uchar addr, uchar type, uchar topic, uchar sub, uchar id);
    bool setting(uint addr, uchar type, uchar topic, uchar sub, uchar id, uint value);
    QVariant log_fun(uchar type, uchar fc, uint id, uint cnt);
};

#endif // WEB_OBJ_H
