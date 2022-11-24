#ifndef WEB_HTTP_H
#define WEB_HTTP_H
#include "web_rpc.h"

struct sWeb_Cfg{
    int http_en = 1;
    int http_port = 80;
    int http_redirect = 0;

    int https_en = 1;
    int https_port = 443;
};

class Web_Http: public Web_Rpc
{
    Q_OBJECT
public:
    Web_Http(QObject *parent = nullptr);
    ~Web_Http();
    static sWeb_Cfg cfg;

    static void mgr_init();
private:
    static void process_json_message(mg_connection *c, mg_str &frame);
    static void fn(mg_connection *c, int ev, void *ev_data, void *fn_data);
    static void process_json_reply(mg_connection *c, const mg_str &frame, char *result);
    static void mgr_upload_big_file(mg_connection **c, mg_http_message **hm, int *state, FILE **fp, const char *path , char *file_path);
    static void mgr_upload_small_file(struct mg_connection **c , struct mg_http_message **hm ,FILE **fp,const char *path, char *file_path);
    static void mgr_download_file(struct mg_connection *c, struct mg_http_message *hm, const char *path);
private slots:
    void run();
};

#endif // WEB_HTTP_H

