#ifndef SQL_STATEMENT_H
#define SQL_STATEMENT_H
#include <QtCore>
#include <QtSql>

class Sql_Statement
{
public:
    explicit Sql_Statement();
    virtual bool createTable();
    virtual QString tableName() = 0;    
    virtual QStringList headList() = 0;

    bool remove(int id);
    bool removeMinIds(int id);
    bool removeMaxIds(int id);    
    bool remove(const QString &condition);
    bool remove(const QString &name, const QString &vale);

    int maxId();
    int idBySql(const QString &sql);
    int maxId(const QString &condition);
    int maxId(const QString &column_name, const QString &condition);

    int minId();
    int minId(const QString &condition);
    int minId(const QString &column_name, const QString &condition);

    int counts();
    int counts(const QString &name);
    int contains(const QString &name, const QString &value);
    int counts(const QString &column_name, const QString &condition);
    bool countsRemove(int count);

    bool updateColumn(const QString& column_name, QString value, const QString &condition);
    bool updateColumn(const QString& column_name, double value, const QString &condition);
    QVariantList listColumn(const QString &column_name, const QString &condition);
    QVariantList selectIds(const QString &condition="");
    bool clear();

protected:
    bool initDb();
    bool initTableMarking();
    bool sqlQuery(QSqlQuery &query);
    bool sqlQuery(const QString &sql);
    bool sqlQuery(QSqlQuery &query, const QString &sql);
    void setTableMarking(const QString& marking);
    bool throwError(const QSqlError &err);
    QString tableMarking();

protected:
    static QSqlDatabase mDb;
};

#endif // SQL_STATEMENT_H
