/*
 *
 *  Created on: 2022年10月1日
 *      Author: Pmd
 */
#include "web_http.h"

#if (QT_VERSION > QT_VERSION_CHECK(5,15,0))
static const char *s_listen_on = "ws://0.0.0.0:8000";
static const char *s_https_addr = "wss://0.0.0.0:8443";  // HTTPS port
static const char *s_web_root = "/home/lzy/work/NPDU/web";
#else
static const char *s_listen_on = "ws://0.0.0.0:";
static const char *s_https_addr = "wss://0.0.0.0:";  // HTTPS port
static const char *s_web_root = "/usr/data/clever/web";
#endif

sWebCfg Web_Http::cfg;
Web_Http::Web_Http()
{
    QtConcurrent::run(this,&Web_Http::run);
}

Web_Http::~Web_Http()
{
    isRun = false;
}

void Web_Http::process_json_reply(mg_connection *c, const mg_str &frame, char *result)
{
    char *response = mg_mprintf("{%Q:%.*s, %Q:%s}", "id", (int)frame.len, frame.ptr, "result", result);
    if(response) mg_ws_printf(c, WEBSOCKET_OP_TEXT, "%s", response);
    //MG_INFO(("[%.*s] -> [%s]", (int) frame.len, frame.ptr, response));

    free(response);
    free(result);
}

void Web_Http::process_json_message(mg_connection *c, mg_str &frame)
{
    struct mg_str params = mg_str(""), id = mg_str("");
    char *response = nullptr, *result = nullptr;
    int params_len = 0, id_len = 0;

    // Parse websocket message, which should be a JSON-RPC frame like this:
    // { "id": 3, "method": "sum", "params": [1,2] }
    char *method = mg_json_get_str(frame, "$.method");
    int id_off = mg_json_get(frame.ptr, (int) frame.len, "$.id", &id_len);
    int params_off = mg_json_get(frame.ptr, (int) frame.len, "$.params", &params_len);
    params = mg_str_n(frame.ptr + params_off, params_len);
    id = mg_str_n(frame.ptr + id_off, id_len);

    if (method == NULL || id_off < 0 || params_off < 0) {
        // Invalid frame. Return error and include this frame as error message
        response = mg_mprintf("{%Q:{%Q:%d,%Q:%.*Q}", "error", "code", -32700,
                              "message", (int) frame.len, frame.ptr);
    } else if (strcmp(method, "pduMetaData") == 0) {
        result = pduMetaData(params);
    } else if (strcmp(method, "pduReadData") == 0) {
        result = pduReadData(params);
    } else if (strcmp(method, "pduSetData") == 0) {
        result = pduSetData(params);
    } else if (strcmp(method, "pduReadParam") == 0) {
        result = pduReadParam(params);
    }else if (strcmp(method, "pduSetParam") == 0) {
        result = pduSetParam(params);
    } else if (strcmp(method, "pduLogFun") == 0) {
        result = pduLogFun(params);
    }else {
        response = mg_mprintf("{%Q:%.*s, %Q:{%Q:%d,%Q:%Q}", "id", (int) id.len, id.ptr,
                              "error", "code", -32601, "message", "Method not found");
    }

    // Send the response back to the client
    if (response) {
        mg_ws_printf(c, WEBSOCKET_OP_TEXT, "%s", response);
        MG_INFO(("[%.*s] -> [%s]", (int) frame.len, frame.ptr, response)); free(response);
    } else process_json_reply(c, id, result);
    free(method);
}

// This RESTful server implements the following endpoints:
//   /websocket - upgrade to Websocket, and implement websocket echo server
//   any other URI serves static files from s_web_root
void Web_Http::fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    char keyFile[32]={0}, certFile[32]={0};
    static FILE* fp = nullptr; static int state = 0;
    qstrcpy(keyFile, File::keyFile().toLatin1().data());
    qstrcpy(certFile, File::certFile().toLatin1().data());
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (ev == MG_EV_OPEN) {
        // c->is_hexdumping = 1;
    } else if (ev == MG_EV_WS_OPEN) {
        c->label[0] = 'W';  // Mark this connection as an established WS client
    } else if (ev == MG_EV_ACCEPT && fn_data != NULL) {
        struct mg_tls_opts opts;
        // opts.ca = "ca.pem";
        memset(&opts , 0 , sizeof(opts));
        opts.cert = certFile;
        opts.certkey = keyFile;
        mg_tls_init(c, &opts);
    } else if (ev == MG_EV_HTTP_MSG) {
        if (mg_http_match_uri(hm, "/websocket")) {
            // Upgrade to websocket. From now on, a connection is a full-duplex
            // Websocket connection, which will receive MG_EV_WS_MSG events.
            mg_ws_upgrade(c, hm, NULL);
        } else if(mg_http_match_uri(hm, "/upload")){
            MG_INFO(("Got all %lu bytes!", (unsigned long) hm->body.len));
            MG_INFO(("Query string: [%.*s]", (int) hm->query.len, hm->query.ptr));
            MG_INFO(("Body:\n%.*s", (int) hm->body.len, hm->body.ptr));
            char file_path[256], name[256];
            mg_http_get_var(&hm->query, "name", name, sizeof(name));
            printf("%s \n", name);
            if (name[0] == '\0') {
                mg_http_reply(c, 400, "", "%s", "name required");
            } else {
                mg_snprintf(file_path, sizeof(file_path), "/tmp/%s", name);
            }
            printf("%s \n", name);
            if((int) hm->query.len > 5){
                printf("%s \n", file_path);
                fp = fopen(file_path , "w+b");
            }
            fwrite(hm->body.ptr ,(int) hm->body.len, 1 , fp);
            fclose(fp);//
            fp = NULL;//
            mg_http_reply(c, 200, "", "ok (%lu)\n", (unsigned long) hm->body.len);
        } else if(mg_http_match_uri(hm, "/cert-client.pem")){
            struct mg_http_serve_opts opts;

            memset(&opts , 0 , sizeof(opts));
            //opts.mime_types = "foo=a/b,pem=c/d";
            opts.mime_types = "foo=a/b,txt=c/d";
            mg_http_serve_file(c , hm , "/etc/ssl/certs/cert.pem" , &opts);
            //mg_http_serve_file(c , hm , "/usr/data/clever/app/json_server_log.txt" , &opts);
        }else if(mg_http_match_uri(hm, "/key-client.pem")){
            struct mg_http_serve_opts opts;

            memset(&opts , 0 , sizeof(opts));
            //opts.mime_types = "foo=a/b,pem=c/d";
            opts.mime_types = "foo=a/b,txt=c/d";
            mg_http_serve_file(c , hm , "/etc/ssl/certs/key.pem" , &opts);
        }
        else {
            // Serve static files
            struct mg_http_serve_opts opts={.root_dir = s_web_root};
            mg_http_serve_dir(c, hm, &opts);
        }
    }else if (ev == MG_EV_HTTP_CHUNK && mg_http_match_uri(hm, "/upload")) {
        MG_INFO(("Got chunk len %lu", (unsigned long) hm->chunk.len));
        MG_INFO(("Query string: [%.*s]", (int) hm->query.len, hm->query.ptr));
        MG_INFO(("Chunk data:\n%.*s", (int) hm->chunk.len, hm->chunk.ptr));


        if(state == 0){
            char file_path[256], name[256];
            mg_http_get_var(&hm->query, "name", name, sizeof(name));
            printf("%s \n", name);
            if (name[0] == '\0') {
                mg_http_reply(c, 400, "", "%s", "name required");
            } else {
                mg_snprintf(file_path, sizeof(file_path), "/tmp/%s", name);
            }
            if((int) hm->query.len > 5){
                printf("%s \n", file_path);
                fp = fopen(file_path , "w+b");
                state = 1;
            }
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
    }
    else if (ev == MG_EV_WS_MSG) {
        // Got websocket frame. Received data is wm->data
        struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
        process_json_message(c, wm->data);
    }
    (void) fn_data;
}


void Web_Http::run()
{
    struct mg_mgr mgr;   // Event manager
    mg_mgr_init(&mgr);  // Init event manager

#if (QT_VERSION > QT_VERSION_CHECK(5,15,0))
    printf("Starting WS listener on %s/websocket\n", s_listen_on);
    mg_http_listen(&mgr, s_listen_on, fn, NULL);  // Create HTTP listener
    mg_http_listen(&mgr, s_https_addr, fn, (void *) 1);  // HTTPS listener
#else
    if(cfg.http_en) {
        QString url = s_listen_on + QString::number(cfg.http_port);
        mg_http_listen(&mgr, url.toStdString().c_str(), fn, NULL);
    }

    if(cfg.https_en) {
        QString url = s_https_addr + QString::number(cfg.https_port);
        mg_http_listen(&mgr, url.toStdString().c_str(), fn, (void *) 1);
    }
#endif
    while(isRun) mg_mgr_poll(&mgr, 1000);             // Infinite event loop
    mg_mgr_free(&mgr);
}

