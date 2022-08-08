#ifndef SET_OUTPUT_H
#define SET_OUTPUT_H

#include "set_login.h"

class Set_Output : public Set_Login
{
public:
    explicit Set_Output();

protected:
    bool relaySet(sDataItem &unit);
    bool outputNameSet(sCfgItem &it, const QVariant &v);
    QString outputName(int addr, int id);

    bool groupNameSet(sCfgItem &it, const QVariant &v);
    QString groupName(int addr, int id);

    bool groupingSet(sCfgItem &it, const QVariant &v);
    QString grouping(int addr, int id);

private:
    bool groupCtrl(sDataItem &unit);
    bool outputCtrl(sDataItem &unit);
    bool outputsCtrl(sDataItem &unit);
    void relayOpLog(const sDataItem &it);
    void opNameLog(const sCfgItem &it, const QVariant &v);
    void writeOpName(int fc, int id, const QVariant &name);
};

#endif // SET_OUTPUT_H
