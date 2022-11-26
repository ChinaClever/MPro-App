#ifndef REDIS_OBJ_H
#define REDIS_OBJ_H

#include "qredisclient/redisclient.h"

struct sRedisCfg
{
    sRedisCfg() {
        host = "192.168.1.117";
        port = 6379;
        pwd = "123456";
        key = "pdu";
        db = 0;
        sec = 5;
        en = 1;
        alive = 60;
        subscribe = "set";
        isConnect = false;
    }

    int en;
    QString host; //服务地址
    QString pwd; // 密码
    int port; // 端口号
    int db; // db编号
    int sec;
    int alive; // 过期时间
    QString key; // 设备key
    QString subscribe; // 订阅频道


    bool isConnect;
    QString error;
};

class Redis_Obj : public QObject
{
    Q_OBJECT
public:
    explicit Redis_Obj(QObject *parent = nullptr);
    static sRedisCfg redisCfg;

    bool expipe(const QByteArray &key, int sec);
    bool set(const QByteArray &key, const QMap<QByteArray, QVariant> &map);
    bool set(const QByteArray &key, const QByteArray &field, const QByteArray &value);

protected:
    void subscribe();
    bool connectServer();
    void initConnectionConfig();

private slots:
    void onConnected();
    void onDisconnected();
    void onError(const QString &);

private:
    RedisClient::Connection *mRedis;
    RedisClient::Connection *mSubscribe;
    RedisClient::ConnectionConfig *mConfig;
};

#endif // REDIS_OBJ_H
