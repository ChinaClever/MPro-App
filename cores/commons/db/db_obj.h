#ifndef DB_OBJ_H
#define DB_OBJ_H
/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "sql_select.h"

class Db_Tran
{
public:
    Db_Tran() {QSqlDatabase::database().transaction();}
    ~Db_Tran() {QSqlDatabase::database().commit();}
};

struct Db_ObjItem {
    Db_ObjItem():id(0){
        QDateTime dateTime = QDateTime::currentDateTime();
        datetime = dateTime.toString("yyyy-MM-dd hh:mm:ss");
    }
    int id; QString datetime;
};


template <typename T>
class Db_Obj : public Sql_Select<T>
{
public:
    Db_Obj() {mHeadList << "dtime";}
    QStringList headList() override {return mHeadList;}
    virtual bool insertItem(const T &item) {
        QString placeholder, val; for(auto &it: mHeadList) {
            placeholder += it +","; val += ":" + it +",";
        } placeholder.chop(1); val.chop(1);
        QString sql = QString("insert into %1 (%2) values(%3)");
        return modifyItem(item, sql.arg(tableName(),placeholder,val));
    }

    virtual bool updateItem(const T &item) {
        QString val; for(auto &it: mHeadList) {
            val += it + " = :" + it +",";
        } val.chop(1); val += " where id  = :id";
        QString sql = "update %1 set %2";
        return modifyItem(item,sql.arg(tableName(),val));
    }

    QByteArray toPageJson(const QVector<T> &its, int minId) {
        QJsonObject json; if(its.size()) {
            int min = its.first().id-minId;
            int max = its.last().id-minId;
            json.insert("table", tableJson(min, max));
        }
        QJsonObject logs; for(auto &it: its) {
            logs.insert(QString::number(it.id-minId), itemJson(it));
        }
        json.insert("logs", logs);
        QJsonDocument doc; doc.setObject(json);
        return doc.toJson(QJsonDocument::Compact);
    }

    QByteArray toOnceJson(const T &it, int minId) {
        QJsonObject json;
        json.insert(QString::number(it.id-minId), itemJson(it));
        QJsonDocument doc; doc.setObject(json);
        return doc.toJson(QJsonDocument::Indented);
    }

    bool jsonWriteFile(const QByteArray &array) {
        bool ret = true; QFile file("load.json");
        if(file.open(QFile::WriteOnly)){
            QTextStream in(&file); in << QString(array);
        } else { ret = false; qDebug() << file.errorString();}
        file.close(); return ret;
    }

protected:
    virtual QString tableName() = 0;
    virtual bool modifyItem(const T &, const QString &cmd)=0;

protected:
    virtual QJsonArray itemJson(const T &it) = 0;
    virtual QJsonObject tableJson(int min, int max) {
        QJsonObject obj; obj.insert("name", tableName());
        obj.insert("id_min", min); obj.insert("id_max", max);
        QJsonArray title; for(auto &it: mHeadList) title << it;
        obj.insert("title", title); return obj;
    }

protected:
    QString tableTitle;
    QStringList mHeadList;
};

#endif // DB_OBJ_H
