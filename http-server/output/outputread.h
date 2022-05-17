#ifndef OUTPUTREAD_H
#define OUTPUTREAD_H
#include <QObject>
#include <stdarg.h>
#include <stdlib.h>
#include "mjson.h"
#include "ipc_outputclient.h"

extern std::vector<std::string *> gVeStr;
extern IPC_OutputClient *gIpc_RelayClientObj;


class OutputRead : public QObject
{
    Q_OBJECT
    explicit OutputRead(QObject *parent = nullptr);
public:
    static OutputRead *bulid(QObject *parent = nullptr);
    static void output_size_value(struct jsonrpc_request * r);
    static void output_name_value(struct jsonrpc_request * r);
    static void output_relay_status(struct jsonrpc_request * r);
    static void output_relay_ctrl(struct jsonrpc_request * r);
    static void get(struct jsonrpc_request * r);
    static void mjson_get_mutilnumbers(struct jsonrpc_request * r ,int num , std::vector<double> & ans);

};




#endif  // OUTPUTREAD_H
