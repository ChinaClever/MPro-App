#ifndef SET_OUTPUT_H
#define SET_OUTPUT_H

#include "set_info.h"

class Set_Output : public Set_Info
{
public:
    explicit Set_Output();

protected:
    bool relaySet(sDataItem &unit);
    bool outputNameSet(sStrItem &it);
    QString outputName(int addr, int id);

private:
    bool outputCtrl(sDataItem &unit);
    void opNameLog(const sStrItem &it);
    void relayOpLog(const sDataItem &it);
    void writeOpName(int id, const QString &name);
};

#endif // SET_OUTPUT_H
