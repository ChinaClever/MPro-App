#ifndef OUTPUTREAD_H
#define OUTPUTREAD_H
#include "jsonrpcobj.h"
#include "ipc_outputclient.h"

class OutputRead : public JsonRpcObj
{
public:
    static void bulid();
    static void output_size_value(struct jsonrpc_request * r);
    static void output_name_value(struct jsonrpc_request * r);
    static void output_relay_status(struct jsonrpc_request * r);
    static void output_relay_ctrl(struct jsonrpc_request * r);
};




#endif  // OUTPUTREAD_H
