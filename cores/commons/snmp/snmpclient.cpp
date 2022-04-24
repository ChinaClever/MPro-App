/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "snmpclient.h"

SnmpClient::SnmpClient(QObject *parent)
    : QObject{parent}
{
    mTimeout = 1000;
    m_snmp_client =  new QtSnmpClient(this);
    m_snmp_client->setCommunity( "public" );
    m_snmp_client->setReponseTimeout(mTimeout);
    connect( m_snmp_client,SIGNAL(responseReceived(qint32,QtSnmpDataList)), this, SLOT(onResponseReceived(qint32,QtSnmpDataList)) );
    connect( m_snmp_client, SIGNAL(requestFailed(qint32)), this, SLOT(onRequestFailed(qint32)));
    setAddress("192.168.1.163");
}

void SnmpClient::setAddress(const QString& ip)
{
    m_snmp_client->setAgentAddress(QHostAddress(ip));
}

bool SnmpClient::setValue(const QString& oid,const QVariant &var)
{
    bool ret = !m_snmp_client->isBusy();
    if(ret) {
        int type = QtSnmpData::STRING_TYPE;
        if(var.type() == QVariant::Int) type = QtSnmpData::INTEGER_TYPE;
        m_snmp_client->setValue("private", oid, type, var.toByteArray());
    }
    return ret;
}

QtSnmpDataList SnmpClient::requestValue(const QStringList& oids)
{
    bool ret = m_snmp_client->isBusy();
    if(!ret) {
        m_snmp_client->requestValues(oids);
        waitForRespond();
    }else mValues.clear();

    return mValues;
}

QtSnmpDataList SnmpClient::requestValue(const QString& oid)
{
    bool ret = m_snmp_client->isBusy();
    if(!ret) {
        m_snmp_client->requestSubValues(oid);
        waitForRespond();
    } else mValues.clear();

    return mValues;
}


bool SnmpClient::waitForRespond()
{
    bool ret = isErr = false;  mValues.clear();
    for(int i=0; i<=mTimeout; i+=100) {
        if(mValues.size() || isErr) {
            ret = true; break;
        } else {
            cm::mdelay(100);
        }
    }

    return ret;
}

void SnmpClient::onResponseReceived(const qint32, const QtSnmpDataList& values )
{
    mValues = values; //for(const auto& value : values)  qDebug() << value.address() << value.data();
}

void SnmpClient::onRequestFailed( const qint32 request_id )
{
    isErr = true; qDebug() << "Error: SnmpClient onRequest Failed" << request_id;
}
