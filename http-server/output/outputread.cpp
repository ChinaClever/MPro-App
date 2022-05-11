#include "outputread.h"

OutputRead::OutputRead(QObject *parent)
{

}


OutputRead *OutputRead::bulid(QObject *parent)
{
    static OutputRead *sington = nullptr;
    if(!sington) {
        sington = new OutputRead(parent);
    }
    return sington;
}



void OutputRead::output_size_value(struct jsonrpc_request * r)
{
    double addr = 0 , index = 0 , a = 0;
    char type[20]="output_size";
    mjson_get_number(r->params, r->params_len, "$[0]", &addr);
    printf("addr: %d len:%d\n" , (int)addr , (int) r->params_len);
    a = (int)addr;
    index = 1;
    addr = 24;

    jsonrpc_return_success(r , "[%g,%g,%Q,%g]" , a ,  index, type , addr  );
}

void OutputRead::output_name_value(struct jsonrpc_request * r)
{

    double addr = 0 , index = 1;
    mjson_get_number(r->params, r->params_len, "$[0]", &addr);
    mjson_get_number(r->params, r->params_len, "$[1]", &index);
    index = (int) index;
    char type[20] = "output_name";
    if(index > 42 || index < 1) return;
    printf("addr: %d ---- index: %d-----%s\n" ,(int) addr ,(int) index-1 ,  (*gVeStr[index-1]).c_str());
    jsonrpc_return_success(r , "[%g,%g,%Q,%Q]" , addr , index , type , (*gVeStr[index-1]).c_str() );
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
    double ctrl = 1;
    //mjson_get_number(r->params, r->params_len, "$[0]", &addr);
    //mjson_get_number(r->params, r->params_len, "$[1]", &id);
    //mjson_get_number(r->params, r->params_len, "$[2]", &ctrl);

    std::vector<double> ans;
    mjson_get_mutilnumbers(r , 3 , ans);
    printf("addr: %f ---- id: %f-----ctrl: %f\n" , ans[0] ,ans[1] , ans[2]);

    IPC_RelayClient::bulid()->ctrl( (int) ans[0] , (int) ans[1] , (int)ans[2] );
    char type[20] = "ctrlop_state";
    jsonrpc_return_success(r , "[%g,%g,%Q,%g]" , ans[0] ,  ans[1] , type , ctrl );
}

void OutputRead::mjson_get_mutilnumbers(struct jsonrpc_request * r ,int num , std::vector<double> & ans)
{
    ans.resize(num);
    char buffer[10];
    for(int i = 0 ; i < num ; i++)
    {
       sprintf(buffer , "$[%d]" , i);
       mjson_get_number(r->params, r->params_len, buffer, &ans[i]);
    }
}

