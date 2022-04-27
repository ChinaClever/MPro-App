#ifndef OUTPUTREAD_H
#define OUTPUTREAD_H
#include <QObject>
#include <stdarg.h>
#include <stdlib.h>
#include <strings.h>
#include "mjson.h"
#include "ipc_relayclient.h"

extern std::vector<std::string *> gVeStr;
extern IPC_RelayClient *gIpc_RelayClientObj;
extern int gOpBit;

class OutputRead
{
    explicit OutputRead();
public:
    static OutputRead *bulid();
    static void output_size_value(struct jsonrpc_request * r);
    static void output_name_value(struct jsonrpc_request * r);
    static void output_relay_status(struct jsonrpc_request * r);
    static void output_relay_ctrl(struct jsonrpc_request * r);
    static void get(struct jsonrpc_request * r);

};




#endif  // OUTPUTREAD_H
