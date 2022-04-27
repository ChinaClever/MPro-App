#ifndef OUTPUTREAD_H
#define OUTPUTREAD_H
#include <QObject>
#include <stdarg.h>
#include <stdlib.h>
#include <strings.h>
#include "mjson.h"
#include "ipc_relayclient.h"

extern std::vector<std::string *> gveStr;
extern IPC_RelayClient *gipc_RelayClientObj;
extern int gopBit;

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
