#ifndef DB_OP_H
#define DB_OP_H
#include "db_user.h"

struct sOpItem : public Db_ObjItem{
    QString op_src, content;
};

class Db_Op : public Db_Obj<sOpItem>
{
    Db_Op();
public:
    static Db_Op* bulid();
    QString tableName(){return "ops";}

protected:
    QJsonArray itemJson(const sOpItem &it);
    bool modifyItem(const sOpItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,sOpItem &item);
};

#endif // DB_OP_H
