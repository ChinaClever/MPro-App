#ifndef WEB_HTTP_H
#define WEB_HTTP_H
#include "web_rpc.h"

class Web_Http: public Web_Rpc
{
public:
    Web_Http();
     ~Web_Http();
    void run();

protected:
    static void process_json_message(mg_connection *c, mg_str &frame);
    static void fn(mg_connection *c, int ev, void *ev_data, void *fn_data);
    static void process_json_reply(mg_connection *c, const mg_str &frame, char *result);

private:
    bool isRun=true;
};

#endif // WEB_HTTP_H

