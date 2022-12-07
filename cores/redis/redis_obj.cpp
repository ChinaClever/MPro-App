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
}

auto Redis_Obj::initConnectionConfig()
{
    sRedisCfg *cfg = &redisCfg;
    auto config =  RedisClient::ConnectionConfig(cfg->host, cfg->pwd);
    config.setAuth(cfg->pwd); config.setPort(cfg->port);
    return config;
}

void Redis_Obj::disconnect()
{
    //if(mRedis) {delete mRedis;  mRedis = nullptr;}
    //if(mSubscribe) {delete mSubscribe; mSubscribe = nullptr;}
    if(mRedis) mRedis->disconnect();
}

void Redis_Obj::subscribe()
{
    sRedisCfg *cfg = &redisCfg;
    if(mSubscribe || cfg->subscribe.isEmpty()) return ;
    auto cf = initConnectionConfig();
    mSubscribe = new RedisClient::Connection(cf); mSubscribe->connect();
    RedisClient::Command command({"subscribe", cfg->subscribe}, cfg->db);//m_orderId是订阅的频道号
    command.setCallBack(this, [&](RedisClient::Response result, QString err) {
        if(result.isArray()){
            QStringList message = result.value().toStringList();
            redisHandleMessage(message);//处理订阅接收到的消息
            //qDebug() << message << err;
        }
    });

    if(mSubscribe->isConnected()) mSubscribe->command(command);
    else qDebug() << "redis subscribe error";

    //    RedisClient::Response r = mSubscribe->command(command);
    //    QString res = RedisClient::Response::valueToHumanReadString(r.value());
    //    qDebug() << "sssssssssssssssss" << res;
}

bool Redis_Obj::set(const QByteArray &key, const QMap<QByteArray, QVariant> &map)
{
    if(!mRedis) return false;
    bool ret = mRedis->isConnected();
    if(ret) {
        QList<QByteArray> rawCmd;
        rawCmd << "HMSET" << key;
        QMapIterator<QByteArray, QVariant> i(map);
        while (i.hasNext()) {
            i.next();
            rawCmd << i.key();
            rawCmd << i.value().toByteArray();
        }
        RedisClient::Response r =mRedis->command(rawCmd, redisCfg.db);
        QString res = RedisClient::Response::valueToHumanReadString(r.value());
        if(!res.contains("OK")) ret = false;
        //qDebug() << ret << rawCmd << res;
    }

    return ret;
}

bool Redis_Obj::set(const QByteArray &key, const QByteArray &field, const QByteArray &value)
{
    if(!mRedis) return false;
    bool ret = mRedis->isConnected();
    if(ret) {
        QList<QByteArray> rawCmd;
        rawCmd << "HMSET" << key << field << value;
        RedisClient::Response r = mRedis->command(rawCmd, redisCfg.db);
        QString res = RedisClient::Response::valueToHumanReadString(r.value());
        if(!res.contains("OK")) ret = false;
        //qDebug() << ret << rawCmd << res;
    }
    return ret;
}

bool Redis_Obj::expipe(const QByteArray &key, int sec)
{
    if(!mRedis) return false;
    bool ret = mRedis->isConnected();
    if(ret) {
        QList<QByteArray> rawCmd;
        rawCmd << "EXPIRE" << key << QByteArray::number(sec);
        RedisClient::Response r = mRedis->command(rawCmd, redisCfg.db);
        QString res = RedisClient::Response::valueToHumanReadString(r.value());
        if(!res.contains("1")) ret = false;
        //qDebug() << ret << rawCmd << res;
    }
    return ret;
}

bool Redis_Obj::connectServer()
{
    if(mRedis) if(mRedis->isConnected()) return true;
    if(!mRedis) {
        auto cf = initConnectionConfig();
        mRedis = new RedisClient::Connection(cf);
        connect(mRedis, &RedisClient::Connection::error, this, &Redis_Obj::onError);
        connect(mRedis, &RedisClient::Connection::connected, this, &Redis_Obj::onConnected);
        connect(mRedis, &RedisClient::Connection::disconnected, this, &Redis_Obj::onDisconnected);
    }
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
