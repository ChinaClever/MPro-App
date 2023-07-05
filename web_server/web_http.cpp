/*
 *
 *  Created on: 2022年10月1日
 *      Author: Pmd
 */
#include "web_http.h"
#include "cfg_file.h"

#if (QT_VERSION > QT_VERSION_CHECK(5,15,0))
static const char *s_listen_on = "ws://0.0.0.0:8000";
static const char *s_https_addr = "wss://0.0.0.0:8443";  // HTTPS port
static const char *s_web_root = "/home/lzy/work/NPDU/web";
#else
static const char *s_listen_on = "ws://0.0.0.0:";
static const char *s_https_addr = "wss://0.0.0.0:";  // HTTPS port
static const char *s_web_root = "/usr/data/clever/web";
static const char *s_ipv6_listen_on = "ws://[::]:";
static const char *s_ipv6_https_addr = "wss://[::]:";
#endif
#define FILE_LEN 256

sWeb_Cfg Web_Http::cfg;
static struct mg_mgr g_mgr;
Web_Http::Web_Http(QObject *parent) : Web_Rpc{parent}
{
    QTimer::singleShot(1255,this,SLOT(run()));
}

Web_Http::~Web_Http()
{
    mg_mgr_free(&g_mgr);
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
//    int id_off = mg_json_get(frame.ptr, (int) frame.len, "$.id", &id_len);
//    int params_off = mg_json_get(frame.ptr, (int) frame.len, "$.params", &params_len);

    int id_off = mg_json_get(frame, "$.id", &id_len);
    int params_off = mg_json_get(frame, "$.params", &params_len);
    params = mg_str_n(frame.ptr + params_off, params_len);
    id = mg_str_n(frame.ptr + id_off, id_len);

    if (method == NULL || id_off < 0 || params_off < 0) {
        // Invalid frame. Return error and include this frame as error message
        response = mg_mprintf("{%Q:{%Q:%d,%Q:%.*Q}", "error", "code", -32700,
                              "message", (int) frame.len, frame.ptr);
    }else if (strcmp(method, "pduMetaData") == 0) {
        result = pduMetaData(params);
    }else if (strcmp(method, "pduReadData") == 0) {
        result = pduReadData(params);
    }else if (strcmp(method, "pduSetData") == 0) {
        result = pduSetData(params);
    }else if (strcmp(method, "pduReadParam") == 0) {
        result = pduReadParam(params);
    }else if (strcmp(method, "pduSetParam") == 0) {
        result = pduSetParam(params);
    }else if (strcmp(method, "pduLogFun") == 0) {
        result = pduLogFun(params);
    }else if (strcmp(method, "pduLogHda") == 0) {
        result = pduLogHda(params);
    }else if (strcmp(method, "execute") == 0) {
        result = execute(params);
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
    char keyFile[126]={0}, certFile[126]={0};
    //static FILE* fp = nullptr; static int state = 0;
    static char file_path[FILE_LEN]={0};
    static char file_name[FILE_LEN]={0};
    qstrcpy(keyFile, File::keyFile().toLatin1().data());
    qstrcpy(certFile, File::certFile().toLatin1().data());
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (ev == MG_EV_OPEN) {
        // c->is_hexdumping = 1;
    } else if (ev == MG_EV_WS_OPEN) {
        c->data[0] = 'W';  // Mark this connection as an established WS client
    } else if (ev == MG_EV_ACCEPT && fn_data != NULL) {
        struct mg_tls_opts opts;
        // opts.ca = "ca.pem";
        memset(&opts , 0 , sizeof(opts));
        opts.cert = certFile;
        opts.certkey = keyFile;
        mg_tls_init(c, &opts);
    } else if (ev == MG_EV_HTTP_MSG) {
        memset(file_path , 0 , sizeof(file_path));
        memset(file_name , 0 , sizeof(file_name));
        if (mg_http_match_uri(hm, "/websocket")) {
            mg_ws_upgrade(c, hm, NULL);
        }else if(mg_http_match_uri(hm, "/index.html/client-cert.pem")){
            mgr_download_file(c , hm , "/index.html/client-cert.pem");
        }else if(mg_http_match_uri(hm, "/index.html/client-key.pem")){
            mgr_download_file(c , hm , "/index.html/client-key.pem");
        }else if(mg_http_match_uri(hm, "/index.html/cfg_batch.zip")){
            mgr_download_file(c , hm , "/index.html/cfg_batch.zip");
        }else if(mg_http_match_uri(hm, "/index.html/cfg_backup.zip")){
            mgr_download_file(c , hm , "/index.html/cfg_backup.zip");
        }else if(mg_http_match_uri(hm, "/index.html/diagnosis.zip")){
            mgr_download_file(c , hm , "/index.html/diagnosis.zip");
        }else if(mg_http_match_uri(hm, "/index.html/modbus.xlsx")){
            mgr_download_file(c , hm , "/index.html/modbus.xlsx");
        }else if(mg_http_match_uri(hm, "/index.html/snmp.mib")){
            mgr_download_file(c , hm , "/index.html/snmp.mib");
        }else if(mg_http_match_uri(hm, "/upload")){
            //mgr_upload_small_file(&c , &hm  , &fp , "/usr/data/clever/certs/%s",file_path);
            mg_http_get_var(&hm->query , "name" , file_name , sizeof(file_name));
            if(file_name[0] == '\0'){
                mg_http_reply(c , 400 , "","%s","name required");
            }else{
                mg_snprintf(file_path , sizeof(file_name) , "/usr/data/clever/certs/%s",file_name);
                mg_http_upload(c , hm , &mg_fs_posix , mg_remove_double_dots(file_path) , 99999);
                system("pdu_cmd pduCfgSet 32 32 1");
            }
            memset(file_path , 0 , sizeof(file_path));
            memset(file_name , 0 , sizeof(file_name));
        }else if(mg_http_match_uri(hm, "/upload_fw")){
            //mgr_upload_small_file(&c , &hm  , &fp , "/usr/data/upload/%s",file_path);
            mg_http_get_var(&hm->query , "name" , file_name , sizeof(file_name));
            if(file_name[0] == '\0'){
                mg_http_reply(c , 400 , "","%s","name required");
            }else{
                mg_snprintf(file_path , sizeof(file_name) , "/usr/data/upload/%s",file_name);
                mg_http_upload(c , hm , &mg_fs_posix , mg_remove_double_dots(file_path) , 30*1024*1024);
            }
            if(file_path[0] != '\0') Web_Obj::bulid()->app_upgrade(file_path);
            memset(file_path , 0 , sizeof(file_path));
            memset(file_name , 0 , sizeof(file_name));
        }else if(mg_http_match_uri(hm, "/upload_batch")){
            //            mgr_upload_small_file(&c , &hm  , &fp , "/usr/data/upload/%s" , file_path);
            //            Web_Obj::bulid()->restores(2,file_path);
            mg_http_get_var(&hm->query , "name" , file_name , sizeof(file_name));
            if(file_name[0] == '\0'){
                mg_http_reply(c , 400 , "","%s","name required");
            }else{
                mg_snprintf(file_path , sizeof(file_name) , "/usr/data/upload/%s",file_name);
                mg_http_upload(c , hm , &mg_fs_posix , mg_remove_double_dots(file_path) , 2*1024*1024);
            }
            if(file_path[0] != '\0') Web_Obj::bulid()->restores(2,file_path);
            memset(file_path , 0 , sizeof(file_path));
            memset(file_name , 0 , sizeof(file_name));

        }else if(mg_http_match_uri(hm, "/upload_backup")){
            //mgr_upload_small_file(&c , &hm  , &fp , "/usr/data/upload/%s" , file_path);
            //Web_Obj::bulid()->restores(1,file_path);
            mg_http_get_var(&hm->query , "name" , file_name , sizeof(file_name));
            if(file_name[0] == '\0'){
                mg_http_reply(c , 400 , "","%s","name required");
            }else{
                mg_snprintf(file_path , sizeof(file_name) , "/usr/data/upload/%s",file_name);
                mg_http_upload(c , hm , &mg_fs_posix , mg_remove_double_dots(file_path) , 2*1024*1024);
            }
            if(file_path[0] != '\0') Web_Obj::bulid()->restores(1,file_path);
        }else if(mg_http_match_uri(hm, "/upload_logo")){
            //mgr_upload_small_file(&c , &hm  , &fp , "/usr/data/clever/cfg/%s" , file_path);
            mg_http_get_var(&hm->query , "name" , file_name , sizeof(file_name));
            if(file_name[0] == '\0'){
                mg_http_reply(c , 400 , "","%s","name required");
            }else{
                mg_snprintf(file_path , sizeof(file_name) , "/usr/data/clever/cfg/%s",file_name);
                mg_http_upload(c , hm , &mg_fs_posix , mg_remove_double_dots(file_path) , 2*1024*1024);
                QString str = "mv " + QString(file_path) + " /usr/data/clever/cfg/logo.png";
                system(str.toStdString().c_str());
            }
            memset(file_path , 0 , sizeof(file_path));
            memset(file_name , 0 , sizeof(file_name));
        }else {
            // Serve static files
            struct mg_http_serve_opts opts={.root_dir = s_web_root};
            mg_http_serve_dir(c, hm, &opts);
        }
    }else if (ev == MG_EV_WS_MSG) {
        // Got websocket frame. Received data is wm->data
        struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
        process_json_message(c, wm->data);
    }
    (void) fn_data;
}

void Web_Http::mgr_download_file(struct mg_connection *c,struct mg_http_message *hm,const char *path)
{
    struct mg_http_serve_opts opts;
    memset(&opts , 0 , sizeof(opts));
    opts.mime_types = "foo=a/b,txt=c/d";
    if(0 == strcmp("/index.html/client-cert.pem" , path)){
        mg_http_serve_file(c , hm , File::certFile().toLatin1().data() , &opts);
    }else if(0 == strcmp("/index.html/client-key.pem" , path)){
        mg_http_serve_file(c , hm , File::keyFile().toLatin1().data() , &opts);
    }else if(0 == strcmp("/index.html/cfg_batch.zip" , path)){
        mg_http_serve_file(c , hm , Web_Obj::bulid()->backup(2).toLatin1().data() , &opts);
    }else if(0 == strcmp("/index.html/cfg_backup.zip" , path)){
        mg_http_serve_file(c , hm , Web_Obj::bulid()->backup(1).toLatin1().data() , &opts);
    }else if(0 == strcmp("/index.html/diagnosis.zip" , path)){
        mg_http_serve_file(c , hm , Web_Obj::bulid()->diag().toLatin1().data() , &opts);
    }else if(0 == strcmp("/index.html/modbus.xlsx" , path)){
        mg_http_serve_file(c , hm , "/usr/data/clever/doc/modbus.xlsx" , &opts);
    }else if(0 == strcmp("/index.html/snmp.mib" , path)){
        opts.mime_types = "mib=a/b,txt=c/d";
        mg_http_serve_file(c , hm , "/usr/data/clever/doc/snmp.mib" , &opts);
    }
}



void Web_Http::mgr_init()
{
    mg_mgr *mgr = &g_mgr;
    mg_mgr_init(mgr);  // Init event manager
#if (QT_VERSION > QT_VERSION_CHECK(5,15,0))
    printf("Starting WS listener on %s/websocket\n", s_listen_on);
    mg_http_listen(mgr, s_listen_on, fn, NULL);  // Create HTTP listener
    mg_http_listen(mgr, s_https_addr, fn, (void *) 1);  // HTTPS listener
#else
    QString en = Web_Obj::bulid()->getIpv6En();
    if(cfg.http_en) {
        if(en == "0"){
            QString url = s_listen_on + QString::number(cfg.http_port);
            mg_http_listen(mgr, url.toStdString().c_str(), fn, NULL);
        }else{
            QString url = s_ipv6_listen_on + QString::number(cfg.http_port);
            mg_http_listen(mgr, url.toStdString().c_str(), fn, NULL);
        }
    }

    if(cfg.https_en) {
        if(en == "0"){
            QString url = s_https_addr + QString::number(cfg.https_port);
            mg_http_listen(mgr, url.toStdString().c_str(), fn, (void *) 1);
        }else{
            QString url = s_ipv6_https_addr + QString::number(cfg.https_port);
            mg_http_listen(mgr, url.toStdString().c_str(), fn, (void *) 1);
        }
    }
#endif
}

void Web_Http::run()
{
    mg_mgr *mgr = &g_mgr;   // Event manager
    mg_mgr_poll(mgr, 100); // Infinite event loop
    QTimer::singleShot(5,this,SLOT(run()));
    //qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
}

