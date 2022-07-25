#ifndef RPC_XML_H
#define RPC_XML_H

#include "rpc_json.h"
#include "maia/maiaXmlRpcServer.h"

class Rpc_Xml : public QObject
{
public:
    explicit Rpc_Xml(QObject *parent = nullptr);
    bool startXmlRpc(int en, int port=8082);

private:
    void addMethods();

private:
    MaiaXmlRpcServer *mXml=nullptr;
};

#endif // RPC_XML_H
