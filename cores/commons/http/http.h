#ifndef HTTP_H
#define HTTP_H
#include <JQNet>

class Http
{
public:
    static QPair<bool, QByteArray> post(const QString &url, const QByteArray &body, const int &timeout=1);
    static QPair<bool, QByteArray> put(const QString &url, const QByteArray &body, const int &timeout=1);
};

#endif // HTTP_H
