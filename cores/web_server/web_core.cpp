#include "web_core.h"

#if (QT_VERSION > QT_VERSION_CHECK(5,15,0))
static const char *s_listen_on = "ws://0.0.0.0:8000";
static const char *s_https_addr = "wss://0.0.0.0:8443";  // HTTPS port
#else
static const char *s_listen_on = "ws://0.0.0.0:80";
static const char *s_https_addr = "wss://0.0.0.0:443";  // HTTPS port
#endif

Web_Core::Web_Core()
{
}

Web_Core::~Web_Core()
{
    isRun = false;
}

Web_Core *Web_Core::bulid()
{
    static Web_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new Web_Core();
        QtConcurrent::run(sington,&Web_Core::run);
    }
    return sington;
}

void Web_Core::run()
{
    struct mg_mgr mgr;   // Event manager
    mg_mgr_init(&mgr);  // Init event manager

    printf("Starting WS listener on %s/websocket\n", s_listen_on);
    mg_http_listen(&mgr, s_listen_on, fn, NULL);  // Create HTTP listener
    mg_http_listen(&mgr, s_https_addr, fn, (void *) 1);  // HTTPS listener
    while (isRun) mg_mgr_poll(&mgr, 1000);             // Infinite event loop
    mg_mgr_free(&mgr);
}
