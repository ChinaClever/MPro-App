#ifndef MB_GROUP_H
#define MB_GROUP_H
#include "mb_output.h"

class Mb_Group : public Mb_Output
{
public:
    explicit Mb_Group(QObject *parent = nullptr);

protected:
    void group_update();
    void group_setting(ushort addr, ushort value);

private:
    void group_dataUpdate();
    void group_relayUpdate();
    void group_thresholdUpdate();
    void group_ctrl(int id, ushort value);
};

#endif // MB_GROUP_H
