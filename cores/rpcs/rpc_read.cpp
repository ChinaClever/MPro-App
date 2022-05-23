#include "rpc_read.h"

Rpc_Read::Rpc_Read(QObject *parent)
    : QObject{parent}
{

}

uint Rpc_Read::pduMetaData(uchar addr,  uchar type, uchar topic, uchar sub, uchar id)
{
    // jcon::JsonRpcServer::clientEndpoint()->peerAddress().toString()
    sDataItem *it = &mIt; it->addr = addr; it->type = type;
    it->topic = topic; it->subtopic = sub; it->id = id;
    Set_Core::bulid()->setting(mIt);
    return mIt.value;
}


QString Rpc_Read::printMessage(const QString& msg)
{
    qDebug().noquote() << QString("-> printMessage: '%1'").arg(msg);
    return QString("Return: '%1'").arg(msg);
}

void Rpc_Read::printNotification(const QString &msg) {
    qDebug().noquote() << QString("-> printNotification: '%1'").arg(msg);
}

void Rpc_Read::namedParams(QString& msg, int answer)
{
    qDebug().noquote() << QString("-> namedParams");
    qDebug().noquote() << "  msg: " << msg;
    qDebug().noquote() << "  answer: " << answer;
}
