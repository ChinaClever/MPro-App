#ifndef JSON_RECV_H
#define JSON_RECV_H

#include "json_build.h"

class Json_Recv : public QObject
{
    Q_OBJECT
public:
    explicit Json_Recv(QObject *parent = nullptr);

signals:

private:
    QJsonValue getValue(QJsonObject &object, const QString &key);
    QJsonObject getObject(QJsonObject &object, const QString &key);
    QJsonArray getArray(QJsonObject &object, const QString &key);
    QString getString(QJsonObject &object, const QString &key);
    double getData(QJsonObject &object, const QString &key);

    bool company(QJsonObject &object);
    bool versionNumber(QJsonObject &object);
};

#endif // JSON_RECV_H
