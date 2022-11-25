#ifndef DB_HDA_H
#define DB_HDA_H
#include "db_event.h"

struct sHdaItem : public Db_ObjItem{
    QString addr, type, topic, indexes, value;
};

class Db_Hda : public Db_Obj<sHdaItem>
{
    Db_Hda();
public:
    static Db_Hda* bulid();
    QString tableName() {return "hdas";}

protected:
    QJsonArray itemJson(const sHdaItem &it);
    bool modifyItem(const sHdaItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,sHdaItem &item);
};

#endif // DB_HDA_H
