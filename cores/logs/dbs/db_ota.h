#ifndef DB_OTA_H
#define DB_OTA_H
#include "db_op.h"

struct sOtaItem : public Db_ObjItem{
    QString module, content;
};

class Db_Ota : public Db_Obj<sOtaItem>
{
    Db_Ota();
public:
    static Db_Ota* bulid();
    QString tableName() {return "otas";}

protected:
    QJsonArray itemJson(const sOtaItem &it);
    bool modifyItem(const sOtaItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,sOtaItem &item);
};

#endif // DB_OTA_H
