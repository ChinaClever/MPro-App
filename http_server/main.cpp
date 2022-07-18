#include <QCoreApplication>
#include <thread>
#include "pdurpcobj.h"

#if (QT_VERSION > QT_VERSION_CHECK(5,15,0))
static const char *s_listen_on = "ws://0.0.0.0:8000";
static const char *s_web_root = "/home/lzy/work/NPDU/web";
#else
static const char *s_listen_on = "ws://0.0.0.0:80";
static const char *s_web_root = "/usr/data/clever/web";
#endif
FILE* fp = NULL;
int state = 0;
// This RESTful server implements the following endpoints:
//   /websocket - upgrade to Websocket, and implement websocket echo server
//   any other URI serves static files from s_web_root
//static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
//    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
//    if (ev == MG_EV_OPEN) {
//        // c->is_hexdumping = 1;
//        //printf("MG_EV_OPEN\n");
//    } else if (ev == MG_EV_WS_OPEN) {
//        c->label[0] = 'W';  // Mark this connection as an established WS client
//        // printf("MG_EV_WS_OPEN\n");
//    }else if(ev == MG_EV_HTTP_CHUNK && mg_http_match_uri(hm, "/upload")){
//        MG_INFO(("Got chunk len %lu", (unsigned long) hm->chunk.len));
//        MG_INFO(("Query string: [%.*s]", (int) hm->query.len, hm->query.ptr));
//        // MG_INFO(("Chunk data:\n%.*s", (int) hm->chunk.len, hm->chunk.ptr));
//        mg_http_delete_chunk(c, hm);
//        if (hm->chunk.len == 0) {
//            MG_INFO(("Last chunk received, sending response"));
//            mg_http_reply(c, 200, "", "ok (chunked)\n");
//        }
//    }else if (ev == MG_EV_HTTP_MSG && mg_http_match_uri(hm, "/upload")) {
//        MG_INFO(("Got all %lu bytes!", (unsigned long) hm->body.len));
//        MG_INFO(("Query string: [%.*s]", (int) hm->query.len, hm->query.ptr));
//        // MG_INFO(("Body:\n%.*s", (int) hm->body.len, hm->body.ptr));
//        mg_http_reply(c, 200, "", "ok (%lu)\n", (unsigned long) hm->body.len);
//    } else if (ev == MG_EV_HTTP_MSG) {
//        // printf("MG_EV_HTTP_MSG\n");

//        if (mg_http_match_uri(hm, "/websocket")) {
//            // Upgrade to websocket. From now on, a connection is a full-duplex
//            // Websocket connection, which will receive MG_EV_WS_MSG events.
//            mg_ws_upgrade(c, hm, NULL);

//        } else {
//            // Serve static files
//            struct mg_http_serve_opts opts = {.root_dir = s_web_root};
//            mg_http_serve_dir(c, (mg_http_message *)ev_data, &opts);
//            //printf("mg_http_serve_dir\n");
//        }

//    } else if (ev == MG_EV_WS_MSG) {
//        // Got websocket frame. Received data is wm->data
//        //printf("MG_EV_WS_MSG\n");
//        struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
//        struct mg_str req = wm->data;
//        char *response = NULL;
//        jsonrpc_process(req.ptr, req.len, mjson_print_dynamic_buf, &response, NULL);
//        mg_ws_send(c, response, strlen(response), WEBSOCKET_OP_TEXT);
//        //LOG(LL_INFO, ("[%.*s] -> [%s]", (int) req.len, req.ptr, response));
//        free(response);
//    }
//    (void) fn_data;
//}

char* responseStr(char *response, char *result, struct mg_str id)
{
    response = mg_mprintf("{%Q:%.*s, %Q:%s}", "id", (int) id.len, id.ptr,
                          "result", result);
    free(result);
    return response;
}


static void process_json_message(struct mg_connection *c, struct mg_str frame) {
    struct mg_str params = mg_str(""), id = mg_str("");
    int params_off = 0, params_len = 0, id_off = 0, id_len = 0;
    char *response = NULL;

    // Parse websocket message, which should be a JSON-RPC frame like this:
    // { "id": 3, "method": "sum", "params": [1,2] }
    char *method = mg_json_get_str(frame, "$.method");
    id_off = mg_json_get(frame.ptr, (int) frame.len, "$.id", &id_len);
    params_off = mg_json_get(frame.ptr, (int) frame.len, "$.params", &params_len);
    params = mg_str_n(frame.ptr + params_off, params_len);
    id = mg_str_n(frame.ptr + id_off, id_len);

    if (method == NULL || id_off < 0 || params_off < 0) {
        // Invalid frame. Return error and include this frame as error message
        response = mg_mprintf("{%Q:{%Q:%d,%Q:%.*Q}", "error", "code", -32700,
                              "message", (int) frame.len, frame.ptr);
    } else if (strcmp(method, "pduReadData") == 0) {
        char *result = PduRpcObj::pduReadData(params);
        response = responseStr(response , result , id);
    } else if (strcmp(method, "pduSetData") == 0) {
        char *result = PduRpcObj::pduSetData(params);
        response = responseStr(response , result , id);
    } else if (strcmp(method, "pduReadString") == 0) {
        char *result = PduRpcObj::pduReadString(params);
        response = responseStr(response , result , id);
    }else if (strcmp(method, "pduSetString") == 0) {
        char *result = PduRpcObj::pduSetString(params);
        response = responseStr(response , result , id);
    }else if (strcmp(method, "pduReadCfg") == 0) {
        char *result = PduRpcObj::pduReadCfg(params);
        response = responseStr(response , result , id);
    }else if (strcmp(method, "pduSetCfg") == 0) {
        char *result = PduRpcObj::pduSetCfg(params);
        response = responseStr(response , result , id);
    }else if (strcmp(method, "pduLogFun") == 0) {
        char *result = PduRpcObj::pduLogFun(params);
        response = responseStr(response , result , id);
    }else {
        response =
                mg_mprintf("{%Q:%.*s, %Q:{%Q:%d,%Q:%Q}", "id", (int) id.len, id.ptr,
                           "error", "code", -32601, "message", "Method not found");
    }

    // Send the response back to the client
    if (response) {
        mg_ws_printf(c, WEBSOCKET_OP_TEXT, "%s", response);
        MG_INFO(("[%.*s] -> [%s]", (int) frame.len, frame.ptr, response));
    }

    free(method);
    free(response);
}

// This RESTful server implements the following endpoints:
//   /websocket - upgrade to Websocket, and implement websocket echo server
//   any other URI serves static files from s_web_root
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (ev == MG_EV_OPEN) {
        // c->is_hexdumping = 1;
    } else if (ev == MG_EV_WS_OPEN) {
        c->label[0] = 'W';  // Mark this connection as an established WS client
    } else if (ev == MG_EV_HTTP_MSG) {

        if (mg_http_match_uri(hm, "/websocket")) {
            // Upgrade to websocket. From now on, a connection is a full-duplex
            // Websocket connection, which will receive MG_EV_WS_MSG events.
            mg_ws_upgrade(c, hm, NULL);
        } else {
            // Serve static files
            struct mg_http_serve_opts opts = {.root_dir = s_web_root};
            mg_http_serve_dir(c, hm, &opts);
        }
    }else if (ev == MG_EV_HTTP_CHUNK && mg_http_match_uri(hm, "/upload")) {
        MG_INFO(("Got chunk len %lu", (unsigned long) hm->chunk.len));
        MG_INFO(("Query string: [%.*s]", (int) hm->query.len, hm->query.ptr));
        MG_INFO(("Chunk data:\n%.*s", (int) hm->chunk.len, hm->chunk.ptr));


        if(state == 0){
            char file_path[256]="/tmp/";
            int n = strlen(file_path);
            int y = 0;
            for(int i = 0 ; i < (int)strlen(hm->query.ptr) ; i++){
                if(hm->query.ptr[i] == ' ') {y = i;break;}
            }
            printf("%d \n" , n);
            printf("%s \n", hm->query.ptr);
            int j = n;
            for(int k = 5; k < y ; j++ , k++){
                file_path[j] = hm->query.ptr[k];
            }
            file_path[j]='\0';
            printf("%s \n", file_path);
            fp = fopen(file_path , "w+b");
            state = 1;
        }
        if(state == 1 && hm->chunk.len != 0){
            fwrite(hm->chunk.ptr ,(int) hm->chunk.len, 1 , fp);
        }

        mg_http_delete_chunk(c, hm);
        if (hm->chunk.len == 0) {
            state = 0;//
            fclose(fp);//
            fp = NULL;//
            MG_INFO(("Last chunk received, sending response"));
            mg_http_reply(c, 200, "", "ok (chunked)\n");
        }
    } else if (ev == MG_EV_HTTP_MSG && mg_http_match_uri(hm, "/upload")) {
        MG_INFO(("Got all %lu bytes!", (unsigned long) hm->body.len));
        MG_INFO(("Query string: [%.*s]", (int) hm->query.len, hm->query.ptr));
        MG_INFO(("Body:\n%.*s", (int) hm->body.len, hm->body.ptr));
        mg_http_reply(c, 200, "", "ok (%lu)\n", (unsigned long) hm->body.len);
    }else if (ev == MG_EV_WS_MSG) {
        // Got websocket frame. Received data is wm->data
        struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
        process_json_message(c, wm->data);
    }
    (void) fn_data;
}

int http_main(void) {
    struct mg_mgr mgr;   // Event manager

    mg_mgr_init(&mgr);  // Init event manager

    PduRpcObj::rpc_export();

    printf("Starting WS listener on %s/websocket\n", s_listen_on);
    mg_http_listen(&mgr, s_listen_on, fn, NULL);  // Create HTTP listener
    for (;;) mg_mgr_poll(&mgr, 1000);             // Infinite event loop
    mg_mgr_free(&mgr);                            // Deallocate event manager
    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QObject *p = a.parent();
    IPC_WebClient::bulid(p);
    //qDebug() << cc->opName(0,2);

    std::thread th(http_main);
    th.detach();

    return a.exec();
}
