#ifndef MB_SETTING_H
#define MB_SETTING_H

#include "mb_env.h"

class Mb_Setting : public Mb_Env
{
    Q_OBJECT
public:
    explicit Mb_Setting(QObject *parent = nullptr);

protected:
    void mbSetUpdate();

private:
    void upSetData();

    void addrSet(ushort &v);
    void buzzerSw(ushort &v);
    void drySw(ushort &v);
    void startSet(ushort addr, ushort &value);

    void setAlarmUnit(sAlarmUnit *unit, int offset, ushort value);
    void lineCurSet(ushort addr, ushort &value);
    void outputCurSet(ushort addr, ushort &value);
    void envSet(ushort addr, ushort &value);

    void lineVolSet(ushort addr, ushort &value);
    void lineCrCurSet(ushort addr, ushort &value);
    void outputCrCurSet(ushort addr, ushort &value);
    void powerOnDelay(ushort &value);

    void loopCurMaxSet(ushort addr, ushort &value);
    void loopCurCrMaxSet(ushort addr, ushort &value);
    void loopCurMinSet(ushort addr, ushort &value);

    void timeSet(ushort addr, ushort &value);
    void timeZone(ushort addr, ushort &value);
    void otherSet(ushort addr, ushort &value);
    void relaySet(ushort addr, ushort &value);

    void eleClear(ushort addr);
    void restoreFactoryDefaults();

private slots:
    //void rcvDataSlot(int address, vshort values);
    void registerRecvSlot(int address,ushort value);
};

#endif // MB_SETTING_H
