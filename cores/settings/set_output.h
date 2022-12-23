#ifndef SET_OUTPUT_H
#define SET_OUTPUT_H

#include "set_login.h"

class Set_Output : public Set_Login
{
public:
    explicit Set_Output();

protected:
    bool relaySet(sDataItem &unit);
    QString outputName(int addr, int id);
    bool outputNameSet(sCfgItem &it, const QVariant &v);

    QString outputCfg(sCfgItem &it);
    bool outputSet(sCfgItem &it, const QVariant &v);
    QString grouping(int addr, int id);
    bool groupingSet(sCfgItem &it, const QVariant &v);
    bool groupSet(sCfgItem &it, const QVariant &v);

private:
    bool groupCtrl(sDataItem &unit);
    bool outputCtrl(sDataItem &unit);
    bool outputsCtrl(sDataItem &unit);
    void relayOpLog(const sDataItem &it);
    void opNameLog(const sCfgItem &it, const QVariant &v);
    bool outputSetById(sCfgItem &it, const QVariant &v);
};

#endif // SET_OUTPUT_H
