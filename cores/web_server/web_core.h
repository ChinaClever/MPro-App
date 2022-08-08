#ifndef WEB_CORE_H
#define WEB_CORE_H

#include "web_http.h"

class Web_Core : public Web_Http
{
public:
    explicit Web_Core();
    ~Web_Core();
    static Web_Core *bulid();
    void run();
private:
    bool isRun=true;
};

#endif // WEB_CORE_H
