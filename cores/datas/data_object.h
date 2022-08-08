#ifndef DATA_OBJECT_H
#define DATA_OBJECT_H

#include "op_core.h"

class Data_Object
{
public:
    explicit Data_Object();

protected:
    void loopData(int id, int start, int end);
    void lineData(int id, int start, int end);    

    uint summation(const uint *ptr, int start, int end);
    uint averageValue(const uint *ptr, int start, int end);
    void sumObjData(int id, sObjData &dest, const sObjData &src, const QList<int> &ls);

private:
    void sumAlarmUnit(int id, sAlarmUnit &dest, const sAlarmUnit &src, int start, int end);
    void sumAlarmUnit(int id, sAlarmUnit &dest, const sAlarmUnit &src, const QList<int> &ls);

    void averAlarmUnit(int id, sAlarmUnit &dest, const sAlarmUnit &src, int start, int end);
    void averAlarmUnit(int id, sAlarmUnit &dest, const sAlarmUnit &src, const QList<int> &ls);

    void sumObjData(int id, sObjData &dest, const sObjData &src, int start, int end);
    uint averageValue(const uint *ptr, const QList<int> &ls);
    uint summation(const uint *ptr, const QList<int> &ls);
    uint calPf(int id, sObjData &obj);

protected:
    sDevData *mDev;
};

#endif // DATA_OBJECT_H
