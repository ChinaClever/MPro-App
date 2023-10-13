#ifndef DB_ALARM_H
#define DB_ALARM_H
#include "db_obj.h"

struct sAlarmItem : public Db_ObjItem{
    int addr=0;
    QString alarm_status;
    QString alarm_content;
};

class Db_Alarm : public Db_Obj<sAlarmItem>
{
    Db_Alarm();
public:
    static Db_Alarm* bulid();
    QString tableName(){return "alarms";}

protected:
    QJsonArray itemJson(const sAlarmItem &it);
    bool modifyItem(const sAlarmItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,sAlarmItem &item);
};

#endif // DB_ALARM_H
