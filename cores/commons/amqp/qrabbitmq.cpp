/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "qrabbitmq.h"
#include "sercet_tlscert.h"

sAmqpCfg QRabbitMQ::amqpCfg;
QRabbitMQ::QRabbitMQ(QObject *parent) :
    QObject(parent)
{
    m_client = new QAmqpClient(this);
    connect(this, &QRabbitMQ::sendMsgSig, this, &QRabbitMQ::sendMsg);
    connect(m_client, SIGNAL(connected()), this, SLOT(clientConnected()));
    connect(m_client, SIGNAL(disconnected()), this, SLOT(disconnected()));
    QTimer::singleShot(1125,this,SLOT(start()));
}

QRabbitMQ *QRabbitMQ::bulid(QObject *parent)
{
    static QRabbitMQ* sington = nullptr;
    if(sington == nullptr) {
        sington = new QRabbitMQ(parent);
    }
    return sington;
}

void QRabbitMQ::start()
{
    sAmqpCfg *cfg = &amqpCfg;
    cfg->isConnected = 0; if(cfg->en) {
        m_bindingKeyList.clear();
        if(cfg->port > 0) m_client->setPort(cfg->port);
        if(cfg->host.size()) m_client->setHost(cfg->host);
        if(cfg->username.size()) m_client->setUsername(cfg->username);
        if(cfg->password.size()) m_client->setPassword(cfg->password);
        if(cfg->bindingKey.size()) m_bindingKeyList << cfg->bindingKey;
        if(cfg->virtualHost.size()) m_client->setVirtualHost(cfg->virtualHost);
        if(cfg->ssl) m_client->setSslConfiguration(Sercret_TlsCert::bulid()->sslConfiguration());
        m_client->setAutoReconnect(true, 10);
        m_client->connectToHost();
    } else {
        if(m_client->isConnected()) m_client->disconnectFromHost();
    }
}

void QRabbitMQ::disconnected()
{
    sAmqpCfg *cfg = &amqpCfg;
    cfg->isConnected = 0;
}

void QRabbitMQ::clientConnected()
{
    sAmqpCfg *cfg = &amqpCfg; cfg->isConnected = 1;
    QAmqpExchange *exchange = m_client->createExchange(cfg->name);
    connect(exchange, SIGNAL(declared()), this, SLOT(exchangeDeclared()));
    exchange->declare(QAmqpExchange::Direct);
    qDebug() << "RabbitMQ connect to server ok";
}

void QRabbitMQ::exchangeDeclared()
{
    QAmqpQueue *temporaryQueue = m_client->createQueue();
    connect(temporaryQueue, SIGNAL(declared()), this, SLOT(queueDeclared()));
    connect(temporaryQueue, SIGNAL(messageReceived()), this, SLOT(messageReceived()));
    temporaryQueue->declare(QAmqpQueue::Exclusive);
}

void QRabbitMQ::queueDeclared()
{
    QAmqpQueue *temporaryQueue = qobject_cast<QAmqpQueue*>(sender());
    if (!temporaryQueue) return;
    temporaryQueue->consume(QAmqpQueue::coNoAck); // start consuming
    foreach (QString severity, m_bindingKeyList) //Bind keys
        temporaryQueue->bind(amqpCfg.name, severity);
    //qDebug() << " [*] Waiting for message.";
}

void QRabbitMQ::messageReceived()
{
    QAmqpQueue *temporaryQueue = qobject_cast<QAmqpQueue*>(sender());
    if (!temporaryQueue) return;
    QAmqpMessage message = temporaryQueue->dequeue();
    QString msg = "Recv: [x] " + message.routingKey() + ":" + message.payload();
    //qDebug() << msg;
    emit recvMsgSig(msg.toUtf8());
}

void QRabbitMQ::sendMsg(const QByteArray &msg)
{
    sAmqpCfg *cfg = &amqpCfg;
    QAmqpExchange *exchange = m_client->createExchange(cfg->name);
    exchange->publish(msg, cfg->routingKey);
}

