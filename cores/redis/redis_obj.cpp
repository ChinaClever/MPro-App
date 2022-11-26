/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "redis_obj.h"

sRedisCfg Redis_Obj::redisCfg;
Redis_Obj::Redis_Obj(QObject *parent)
    : QObject{parent}
{
    mRedis = nullptr;
    initRedisClient();
    mSubscribe = nullptr;
    initConnectionConfig();
}

void Redis_Obj::initConnectionConfig()
{
    sRedisCfg *cfg = &redisCfg;
    mConfig = new RedisClient::ConnectionConfig(cfg->host, cfg->pwd);
    mConfig->setAuth(cfg->pwd); mConfig->setPort(cfg->port);
}

void Redis_Obj::subscribe()
{
    if(mSubscribe) return ;
    sRedisCfg *cfg = &redisCfg;
    RedisClient::ConnectionConfig cf (cfg->host, cfg->pwd);
    cf.setAuth(cfg->pwd); cf.setPort(cfg->port);
    mSubscribe = new RedisClient::Connection(cf);
    RedisClient::Command command({"subscribe", cfg->subscribe.toLocal8Bit()}, cfg->db);//m_orderId是订阅的频道号
    command.setCallBack(this, [&](RedisClient::Response result, QString err) {
        if(result.isArray()){
            QStringList message = result.value().toStringList();
            qDebug() << message << err;
            //redisHandleMessage(message);//处理订阅接收到的消息
        }
    });

    //qDebug() << "jjjjjjjjjjjjjjjjjjjjjjjjjjjj" ;
    RedisClient::Response r = mSubscribe->command(command);
    //RedisClient::Response r = mRedis->command(rawCmd, redisCfg.db);
    QString res = RedisClient::Response::valueToHumanReadString(r.value());
    //qDebug() << "sssssssssssssssss" << res;
}

bool Redis_Obj::set(const QByteArray &key, const QMap<QByteArray, QVariant> &map)
{
    bool ret = mRedis->isConnected();
    if(ret) {
        QList<QByteArray> rawCmd;
        rawCmd << "HMSET" << key;
        QMapIterator<QByteArray, QVariant> i(map);
        while (i.hasNext()) {
            i.next();
            rawCmd << i.key();
            rawCmd << i.value().toByteArray();
        }  mRedis->command(rawCmd, redisCfg.db);
    }

    return ret;
}

bool Redis_Obj::set(const QByteArray &key, const QByteArray &field, const QByteArray &value)
{
    bool ret = mRedis->isConnected();
    if(ret) {
        QList<QByteArray> rawCmd;
        rawCmd << "HMSET" << key << field << value;
        RedisClient::Response r = mRedis->command(rawCmd, redisCfg.db);
        QString res = RedisClient::Response::valueToHumanReadString(r.value());
        if(!res.contains("OK")) ret = false; //qDebug() << rawCmd;
    }
    return ret;
}

bool Redis_Obj::expipe(const QByteArray &key, int sec)
{
    bool ret = mRedis->isConnected();
    if(ret) {
        QList<QByteArray> rawCmd;
        rawCmd << "EXPIRE" << key << QByteArray::number(sec);
        RedisClient::Response r = mRedis->command(rawCmd, redisCfg.db);
        QString res = RedisClient::Response::valueToHumanReadString(r.value());
        if(!res.contains("1")) ret = false; //qDebug() << rawCmd;
    }
    return ret;
}

bool Redis_Obj::connectServer()
{
    if(mRedis) return true;
    mRedis = new RedisClient::Connection(*mConfig);
    connect(mRedis, &RedisClient::Connection::error, this, &Redis_Obj::onError);
    connect(mRedis, &RedisClient::Connection::connected, this, &Redis_Obj::onConnected);
    connect(mRedis, &RedisClient::Connection::disconnected, this, &Redis_Obj::onDisconnected);
    return mRedis->connect();
}


void Redis_Obj::onConnected()
{
    redisCfg.isConnect = true;
    qDebug() << "redis connected";
}

void Redis_Obj::onDisconnected()
{
    redisCfg.isConnect = false;
    qDebug() << "redis disconnected";
}

void Redis_Obj::onError(const QString &msg)
{
    redisCfg.error = msg;
    qDebug() << "redis error" << msg;
}
