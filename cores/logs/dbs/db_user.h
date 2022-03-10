#ifndef DB_USER_H
#define DB_USER_H
#include "db_obj.h"

struct sUserItem : public Db_ObjItem{
    sUserItem():jur(0){}
    int jur;
    QString name,pwd,jurisdiction, email,telephone,remarks;
};

class Db_User : public Db_Obj<sUserItem>
{
    Db_User();
public:
    static Db_User* bulid();
    QString tableName(){return "users";}
    int contain(const QString &name);
    sUserItem selItemsByName(const QString& name);
    void removeItemsByName(const QString& name);

protected:
    QJsonArray itemJson(const sUserItem &it);
    bool modifyItem(const sUserItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,sUserItem &item);
};

#endif // DB_USER_H
