#ifndef WEB_OBJ_H
#define WEB_OBJ_H
#include <QtCore>
#include "mongoose.h"
#include "set_core.h"

class Web_Obj
{
public:
    Web_Obj();
    uint getNumber(mg_str &r, int id);
    QString getString(mg_str &r, int id);
    QVector<uint> getNumbers(mg_str &r, int num);

    QString metaData(uint addr);
    QString getCfg(uint type, uint fc, uint addr, uint sub);
    bool setCfg(uint type, uint fc, const QVariant &value, uint addr, uint sub);
    bool setting(uint addr, uint type, uint topic, uint sub, uint id, uint value);
    int getValue(uint addr, uint type, uint topic, uint sub, uint id);
    QVariant log_fun(uint type, uint fc, uint id, uint cnt);
};

#endif // WEB_OBJ_H
