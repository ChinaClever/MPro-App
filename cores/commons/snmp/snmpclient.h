#ifndef SNMPCLIENT_H
#define SNMPCLIENT_H

#include <QtCore>
#include "client/QtSnmpClient.h"
#include "commons.h"

#define MIB_OID_CLEVER  ".1.3.6.1.4.1.30966."
#define IP_MIB_OID  MIB_OID_CLEVER"10.3.2"

class SnmpClient : public QObject
{
    Q_OBJECT
public:
    explicit SnmpClient(QObject *parent = nullptr);
    void setAddress(const QString& ip);
    QtSnmpDataList requestValue(const QString& oid);
    QtSnmpDataList requestValue(const QStringList& oids);
    bool setValue(const QString& oid,const QVariant &var);

private:
    bool waitForRespond();

private slots:
    void onRequestFailed(const qint32 request_id);
    void onResponseReceived(const qint32 request_id, const QtSnmpDataList& );

private:
    bool isErr; int mTimeout;
    QtSnmpDataList mValues;
    QtSnmpClient *m_snmp_client;
};

#endif // SNMPCLIENT_H
