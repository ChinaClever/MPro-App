#include "outputread.h"

OutputRead::OutputRead()
{

}


OutputRead *OutputRead::bulid()
{
    static OutputRead *sington = nullptr;
    if(!sington) {
        sington = new OutputRead();
    }
    return sington;
}



void OutputRead::output_size_value(struct jsonrpc_request * r)
{
    double addr = 0;
    mjson_get_number(r->params, r->params_len, "$[0]", &addr);
    printf("addr: %d\n" , (int)addr);
    addr = 24;
    jsonrpc_return_success(r , "%g" , addr );
}

void OutputRead::output_name_value(struct jsonrpc_request * r)
{

    double addr = 0 , index = 1;
    mjson_get_number(r->params, r->params_len, "$[0]", &addr);
    mjson_get_number(r->params, r->params_len, "$[1]", &index);
    index = (int) index;
    if(index > 42 || index < 1) return;
    printf("addr: %d ---- index: %d-----%s\n" ,(int) addr ,(int) index-1 ,  (*gveStr[index-1]).c_str());
    jsonrpc_return_success(r , "%Q" , (*gveStr[index-1]).c_str() );
}

int aa = 0;
void OutputRead::get(struct jsonrpc_request * r)
{
    double a = 0;
    mjson_get_number(r->params, r->params_len, "$[0]", &a);
    if( aa == 0 ) {a = 221; aa = 1 ;}
    else{ a = 222; aa = 0 ;}
    jsonrpc_return_success(r , "%g" , a );
}

void OutputRead::output_relay_status(struct jsonrpc_request * r)
{
    double addr = 0 , id = 1;
    mjson_get_number(r->params, r->params_len, "$[0]", &addr);
    mjson_get_number(r->params, r->params_len, "$[1]", &id);


    jsonrpc_return_success(r , "%g" , id );
}

void OutputRead::output_relay_ctrl(struct jsonrpc_request * r)
{
    double addr = 0 , id = 1 , ctrl = 1;
    mjson_get_number(r->params, r->params_len, "$[0]", &addr);
    mjson_get_number(r->params, r->params_len, "$[1]", &id);
    mjson_get_number(r->params, r->params_len, "$[2]", &ctrl);
    if( !gipc_RelayClientObj ) return;
    gipc_RelayClientObj->ctrl( addr , id , ctrl );
    jsonrpc_return_success(r , "%g" , 1 );
}


