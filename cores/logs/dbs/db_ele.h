#ifndef DB_ELE_H
#define DB_ELE_H
#include "db_user.h"
struct sEleItem : public Db_ObjItem{
    QString addr, module, ele;
};

class Db_Ele : public Db_Obj<sEleItem>
{
    Db_Ele();
public:
    static Db_Ele* bulid();
    QString tableName(){return "eles";}

protected:
    QJsonArray itemJson(const sEleItem &it);
    bool modifyItem(const sEleItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query, sEleItem &item);
};

#endif // DB_ELE_H
