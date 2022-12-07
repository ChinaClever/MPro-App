#ifndef QRABBITMQ_H
#define QRABBITMQ_H
#include <QtCore>
#include "qamqp/qamqpclient.h"
#include "qamqp/qamqpexchange.h"
#include "qamqp/qamqpqueue.h"

struct sAmqpCfg
{
    sAmqpCfg() {}
    int en; int ssl=0;
    int port = 15672;
    QString host = "http://localhost";
    QString name; // 交换机名称
    QString username;
    QString password;
    QString routingKey;
    QString bindingKey;
    QString virtualHost;
    int isConnected=0;
    int sec;
};

class QRabbitMQ : public QObject
{
    Q_OBJECT
    explicit QRabbitMQ(QObject *parent = 0);
public:
    static QRabbitMQ *bulid(QObject *parent = nullptr);
    static sAmqpCfg amqpCfg;

signals:
    void sendMsgSig(const QByteArray &msg);
    void recvMsgSig(const QByteArray &msg);

public slots:
    void start();
    void sendMsg(const QByteArray &msg);

private slots:
    void clientConnected();
    void exchangeDeclared();
    void queueDeclared();
    void messageReceived();
    void disconnected();

private:
    QAmqpClient *m_client;
    //One client can bind many keys
    QStringList m_bindingKeyList;
};
#endif // QRABBITMQ_H


