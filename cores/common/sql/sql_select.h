#ifndef SQL_SELECT_H
#define SQL_SELECT_H
#include "sql_statement.h"

template <typename T>
class Sql_Select : public Sql_Statement
{
public:
    T selectById(int id) {
        QString sql = QString("where id = %1").arg(id);
        return selectItems(sql).frist();
    }

    QVector<T> selectAll(){return selectItems();}
    QVector<T> selectBetween(const QString& name, int start, int end) {
        QString cmd = QString("where %1 Between %2 and %3");
        return selectItems(cmd.arg(name).arg(start).arg(end));
    }

    QVector<T> selectBetween(int start, int end){return selectBetween("id", start, end);}
    QVector<T> selectBetween(const QString& name, const QString &start, const QString &end){
        QString cmd = QString("where %1 Between \'%2\' and \'%3\'");
        return selectItems(cmd.arg(name,start,end));
    }

    QVector<T> selectByTime(const QString &start, const QString &end) {
         return selectBetween("dtime", start, end);
    }

protected:
    virtual void selectItem(QSqlQuery &,T &) = 0;
    QVector<T> selectItems(const QString &condition="") {
        QVector<T> items; QSqlQuery query(mDb);
        QString sql = QString("SELECT * from %1 %2").arg(tableName(),condition);;
        if(sqlQuery(query, sql)){
            T item; while(query.next()){
                selectItem(query,item); items.append(item);
            }
        }
        return items;
    }

    QVector<T> selectItems(const QString &name, const QString &value) {
        QString sql = "where %1 = \'%2\'";
        return selectItems(sql.arg(name, value));
    }
};

#endif // SQL_SELECT_H
