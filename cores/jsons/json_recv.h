#ifndef JSON_RECV_H
#define JSON_RECV_H

#include "json_build.h"

class Json_Recv : public QObject
{
    Q_OBJECT
    explicit Json_Recv(QObject *parent = nullptr);
public:
    static Json_Recv *bulid(QObject *parent = nullptr);
    bool recv(const QByteArray &msg);

signals:
    void recvSetSig(const sDataItem &it);
    void recvNumStrSig(const sNumStrItem &it);

private:
    QJsonValue getValue(const QJsonObject &object, const QString &key);
    QJsonObject getObject(const QJsonObject &object, const QString &key);
    QJsonArray getArray(const QJsonObject &object, const QString &key);
    QString getString(const QJsonObject &object, const QString &key);
    double getData(const QJsonObject &object, const QString &key);

    bool company(const QJsonObject &object);
    bool versionNumber(const QJsonObject &object);

    bool setDataItem(const QJsonObject &object);
    bool setNumStrItem(const QJsonObject &object);
    bool analyticalData(const QJsonObject &object);
};

#endif // JSON_RECV_H
