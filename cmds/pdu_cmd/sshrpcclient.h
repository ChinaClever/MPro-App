#ifndef SSHRPCCLIENT_H
#define SSHRPCCLIENT_H

#include "jsonrpc_client.h"

class SshRpcClient : public JsonRpc_Client
{
    explicit SshRpcClient(QObject *parent = nullptr);
public:
    static SshRpcClient *bulid(QObject *parent = nullptr);
};

#endif // SSHRPCCLIENT_H
