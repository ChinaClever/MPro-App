#include "outputread.h"

void OutputRead::bulid()
{
    jsonrpc_export("output_size_value" , output_size_value);
    jsonrpc_export("output_name_value" , output_name_value);
    jsonrpc_export("output_relay_status" , output_relay_status);
    jsonrpc_export("output_relay_ctrl" , output_relay_ctrl);
}

void OutputRead::output_size_value(struct jsonrpc_request * r)
{
    const char *type = "output_size";
    double addr = getNumber(r, 0);

    double a = addr;
    double index = 1;
    addr = 24;

    jsonrpc_return_success(r, "[%g,%g,%Q,%g]", a, index, type ,addr);
}

void OutputRead::output_name_value(struct jsonrpc_request * r)
{
    QVector<double> its = getNumbers(r, 2);
    double addr = its.at(0), index = its.at(1);

    const char *type = "output_name";
    if(index > 0 && index < 50) {
        char name[20] = {0};
        sprintf(name , "output%d" , (int)index);
        jsonrpc_return_success(r, "[%g,%g,%Q,%Q]", addr, index, type, name);
    } else {

    }
}

void OutputRead::output_relay_status(struct jsonrpc_request * r)
{
    double addr = getNumber(r, 0);
    double id =  getNumber(r, 1);


    jsonrpc_return_success(r , "%g" , id );
}

void OutputRead::output_relay_ctrl(struct jsonrpc_request * r)
{
    double ctrl = 1;
    QVector<double> ans = getNumbers(r, 3);
    printf("addr: %f ---- id: %f-----ctrl: %f\n" , ans[0] ,ans[1] , ans[2]);

    IPC_OutputClient::bulid()->ctrl( (int) ans[0] , (int) ans[1] , (int)ans[2] );
    char type[20] = "ctrlop_state";
    jsonrpc_return_success(r , "[%g,%g,%Q,%g]" , ans[0] ,  ans[1] , type , ctrl );
}

