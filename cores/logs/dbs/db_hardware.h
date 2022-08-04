#ifndef DB_HARDWARE_H
#define DB_HARDWARE_H
#include "db_ele.h"

struct sHardwareItem : public Db_ObjItem{
    QString module, content;
};

class Db_Hardware : public Db_Obj<sHardwareItem>
{
    Db_Hardware();
public:
    static Db_Hardware* bulid();
    QString tableName() {return "hardwares";}

protected:
    QJsonArray itemJson(const sHardwareItem &it);
    bool modifyItem(const sHardwareItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,sHardwareItem &item);
};

#endif // DB_HARDWARE_H
