#ifndef WS_CLIENT_H
#define WS_CLIENT_H

#include "ws_object.h"

class WS_Client : public WS_Object
{
    Q_OBJECT
public:
    explicit WS_Client(QObject *parent = nullptr);
    bool open(const QString &url, int port);
};

#endif // WS_CLIENT_H
