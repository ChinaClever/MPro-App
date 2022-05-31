#include <QCoreApplication>
#include "sshrpcclient.h"

int main(int argc, char *argv[])
{
    SshRpcClient *rpc = SshRpcClient::bulid();


    rpc->close();

    return 0;
}
