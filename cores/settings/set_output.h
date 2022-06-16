#ifndef SET_OUTPUT_H
#define SET_OUTPUT_H

#include "set_login.h"

class Set_Output : public Set_Login
{
public:
    explicit Set_Output();

protected:
    bool relaySet(sDataItem &unit);
    bool outputNameSet(sNumStrItem &it);
    QString outputName(int addr, int id);

private:
    bool outputCtrl(sDataItem &unit);
    void opNameLog(const sNumStrItem &it);
    void relayOpLog(const sDataItem &it);
    void writeOpName(int id, const QString &name);
};

#endif // SET_OUTPUT_H
