#include "set_service.h"
#include "web_server/web_core.h"

Set_Service::Set_Service()
{

}

int Set_Service::webCfg(int fc)
{
    int ret = 0;
    sWebCfg *it = &Web_Core::cfg;
    switch (fc) {
    case 1: ret = it->http_en; break;
    case 2: ret = it->http_port; break;
    case 3: ret = it->http_redirect; break;
    case 4: ret = it->https_en; break;
    case 5: ret = it->https_port; break;
    default: cout << fc; break;
    }

    return ret;
}

bool Set_Service::webSet(int fc, const QVariant &v)
{
    bool ret = true;
    sWebCfg *it = &Web_Core::cfg;
    QString prefix = "web"; QString key;

    switch (fc) {
    case 1: key = "http_en";  it->http_en = v.toInt(); break;
    case 2: key = "http_port";  it->http_port = v.toInt(); break;
    case 3: key = "http_redirect";  it->http_port = v.toInt();  break;
    case 4: key = "https_en";  it->https_en =v.toInt();  break;
    case 5: key = "https_port";  it->https_port = v.toInt(); break;
    default: ret = false; qDebug() << Q_FUNC_INFO; break;
    }

    Cfg_Obj *cfg = Cfg_Obj::bulid();
    cfg->writeCfg(key, v, prefix);
    return ret;
}
