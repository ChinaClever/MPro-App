#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H
#include <JQNet>

class Http
{
public:
    static QPair<bool, QByteArray> post(const QString &url, const QByteArray &body, const int &timeout=1000);
};

#endif // HTTPCLIENT_H
