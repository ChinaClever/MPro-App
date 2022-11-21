#ifndef DB_EVENT_H
#define DB_EVENT_H
#include "db_alarm.h"

struct sEventItem : public Db_ObjItem{
    QString event_type;
    QString event_content;
};

class Db_Event : public Db_Obj<sEventItem>
{
    Db_Event();
public:
    static Db_Event* bulid();
    QString tableName() {return "events";}

protected:
    QJsonArray itemJson(const sEventItem &it);
    bool modifyItem(const sEventItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,sEventItem &item);
};

#endif // DB_EVENT_H
