#ifndef SSHRPCCLIENT_H
#define SSHRPCCLIENT_H

#include "jsonrpcclient.h"

class SshRpcClient : public JsonRpcClient
{
    explicit SshRpcClient(QObject *parent = nullptr);
public:
    static SshRpcClient *bulid(QObject *parent = nullptr);
};

#endif // SSHRPCCLIENT_H
