#ifndef DB_SYS_H
#define DB_SYS_H
#include "db_op.h"

struct sSysItem : public Db_ObjItem{
    QString module, content;
};

class Db_Sys : public Db_Obj<sSysItem>
{
    Db_Sys();
public:
    static Db_Sys* bulid();
    QString tableName(){return "sys";}

protected:
    QJsonArray itemJson(const sSysItem &it);
    bool modifyItem(const sSysItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,sSysItem &item);
};

#endif // DB_SYS_H
