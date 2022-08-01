#ifndef INTEGR_JSONRECV_H
#define INTEGR_JSONRECV_H
#include "integr_jsonbuild.h"

class Integr_JsonRecv : public QObject
{
    Q_OBJECT
    explicit Integr_JsonRecv(QObject *parent = nullptr);
public:
    static Integr_JsonRecv *bulid(QObject *parent = nullptr);
    bool recv(const QByteArray &msg);

signals:
    void recvSetSig(const sDataItem &it);
    void recvCfgSig(const sCfgItem &it);

public:
    bool checkInput(const QByteArray &msg, QJsonObject &obj);
    QJsonValue getValue(const QJsonObject &object, const QString &key);
    QJsonArray getArray(const QJsonObject &object, const QString &key);
    QJsonObject getObject(const QJsonObject &object, const QString &key);
    QJsonObject getObject(const QByteArray &msg, const QString &key);

    QString getString(const QJsonObject &object, const QString &key);
    QString getString(const QByteArray &msg, const QString &key);

    double getData(const QJsonObject &object, const QString &key);
    double getData(const QByteArray &msg, const QString &key);

    bool company(const QJsonObject &object);
    bool versionNumber(const QJsonObject &object);

    bool setDataItem(const QJsonObject &object);
    bool setCfgItem(const QJsonObject &object);
    bool analyticalData(const QJsonObject &object);
};

#endif // INTEGR_JSONRECV_H
