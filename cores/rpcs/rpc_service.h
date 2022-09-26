#ifndef RPC_SERVICE_H
#define RPC_SERVICE_H
#include "rpc_xml.h"

struct sRpcIt {
    int en = 0;
    int port = 8082;
};

struct sRpcCfg{
    sRpcIt json;
    sRpcIt xml;
};

class Rpc_Service : public QObject
{
    Q_OBJECT
    explicit Rpc_Service(QObject *parent = nullptr);
public:
    static Rpc_Service *bulid(QObject *parent = nullptr);    
    static sRpcCfg rpcCfg;
    bool startJsonRpc();
    bool startXmlRpc();

private slots:
    void initFunSlot();

private:
    Rpc_Xml *mXml;
    Rpc_Json *mJson;
};

#endif // RPC_SERVICE_H
