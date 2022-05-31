#include <QCoreApplication>
#include <QtCore>
#include <thread>
#include "mjson.h"
#include "mongoose.h"
#include "jsonrpcobj.h"
#include "ipc_echoclient.h"

static const char *s_listen_on = "ws://localhost:8000";
static const char *s_web_root = "/home/lzy/work/NPDU/web";
IPC_EchoClient *ipc_echoClient;

//static void sum(struct jsonrpc_request *r) {
//    double a = 0.0, b = 0.0;
//    mjson_get_number(r->params, r->params_len, "$[0]", &a);
//    mjson_get_number(r->params, r->params_len, "$[1]", &b);
//    jsonrpc_return_success(r, "%g", a + b);
//}

//static void multiply(struct jsonrpc_request *r) {
//    double a = 0.0, b = 0.0;
//    mjson_get_number(r->params, r->params_len, "$[0]", &a);
//    mjson_get_number(r->params, r->params_len, "$[1]", &b);
//    jsonrpc_return_success(r, "%g", a * b);
//}

static void pduReadData(struct jsonrpc_request *r) {
    double value = 0.0;
    QVector<double> its = JsonRpcObj::getNumbers(r , 5);
    if(ipc_echoClient){
        value = ipc_echoClient->getValue((int)its.at(0) , (int)its.at(1) , (int)its.at(2) , (int)its.at(3) , (int)its.at(4));
        printf("%.3f  \n" , value);
        jsonrpc_return_success(r , "[%g,%g,%g,%g,%g,%g]" , its.at(0) , its.at(1) , its.at(2) , its.at(3) , its.at(4) , value);
    }
}

static void pduSetData(struct jsonrpc_request *r) {
    double value = 0.0;
    QVector<double> its = JsonRpcObj::getNumbers(r , 6);
    if(ipc_echoClient){
        ipc_echoClient->setting((int)its.at(0) , (int)its.at(1) , (int)its.at(2) , (int)its.at(3) , (int)its.at(4), (int)its.at(5));
        printf("%.3f  \n" , value);
        jsonrpc_return_success(r , "[%g,%g,%g,%g,%g,%g]" , its.at(0) , its.at(1) , its.at(2) , its.at(3) , its.at(4) , 1);
    }
}



// This RESTful server implements the following endpoints:
//   /websocket - upgrade to Websocket, and implement websocket echo server
//   any other URI serves static files from s_web_root
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_OPEN) {
        // c->is_hexdumping = 1;
        //printf("MG_EV_OPEN\n");
    } else if (ev == MG_EV_WS_OPEN) {
        c->label[0] = 'W';  // Mark this connection as an established WS client
       // printf("MG_EV_WS_OPEN\n");
    } else if (ev == MG_EV_HTTP_MSG) {
       // printf("MG_EV_HTTP_MSG\n");
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        if (mg_http_match_uri(hm, "/websocket")) {
            // Upgrade to websocket. From now on, a connection is a full-duplex
            // Websocket connection, which will receive MG_EV_WS_MSG events.
            mg_ws_upgrade(c, hm, NULL);

        } else {
            // Serve static files
            struct mg_http_serve_opts opts = {.root_dir = s_web_root};
            mg_http_serve_dir(c, (mg_http_message *)ev_data, &opts);
            //printf("mg_http_serve_dir\n");
        }

    } else if (ev == MG_EV_WS_MSG) {
        // Got websocket frame. Received data is wm->data
        printf("MG_EV_WS_MSG\n");
        struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
        struct mg_str req = wm->data;
        char *response = NULL;
        jsonrpc_process(req.ptr, req.len, mjson_print_dynamic_buf, &response, NULL);
        mg_ws_send(c, response, strlen(response), WEBSOCKET_OP_TEXT);
        //LOG(LL_INFO, ("[%.*s] -> [%s]", (int) req.len, req.ptr, response));
        free(response);
    }
    (void) fn_data;
}

//static void timer_fn(void *arg) {
//    struct mg_mgr *mgr = (struct mg_mgr *) arg;
//    // Broadcast "hi" message to all connected websocket clients.
//    // Traverse over all connections
//    for (struct mg_connection *c = mgr->conns; c != NULL; c = c->next) {
//        // Send JSON-RPC notifications to marked connections
//        const char *msg = "{\"method\":\"hiya!!\",\"params\":[1,2,3]}";
//        if (c->label[0] == 'W') mg_ws_send(c, msg, strlen(msg), WEBSOCKET_OP_TEXT);
//    }
//}

int http_main(void) {
    struct mg_mgr mgr;   // Event manager
    //struct mg_timer t1;  // Timer

    mg_mgr_init(&mgr);  // Init event manager
    //mg_timer_init(&t1, 5000, MG_TIMER_REPEAT, timer_fn, &mgr);  // Init timer

    jsonrpc_init(NULL, NULL);         // Init JSON-RPC instance
//    jsonrpc_export("sum", sum);       // And export a couple
//    jsonrpc_export("mul", multiply);  // of RPC functions

    jsonrpc_export("pduReadData", pduReadData);  // of RPC functions
    //jsonrpc_export("pduReadString", pduReadString);
    jsonrpc_export("pduSetData", pduSetData);
    //jsonrpc_export("pduSetString", pduSetString);


    printf("Starting WS listener on %s/websocket\n", s_listen_on);

    mg_http_listen(&mgr, s_listen_on, fn, NULL);  // Create HTTP listener
    for (;;) mg_mgr_poll(&mgr, 1000);             // Infinite event loop
/*    mg_timer_free(&t1);*/                           // Free timer resources
    mg_mgr_free(&mgr);                            // Deallocate event manager
    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QObject *p = a.parent();
    ipc_echoClient = new IPC_EchoClient(p);

    std::thread th(http_main);
    th.detach();

    return a.exec();
}
