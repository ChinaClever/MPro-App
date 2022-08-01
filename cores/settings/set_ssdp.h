#ifndef SET_SSDP_H
#define SET_SSDP_H

#include "ssdp_server.h"
#include "set_core.h"

class Set_Ssdp : public QObject
{
    Q_OBJECT
    explicit Set_Ssdp(QObject *parent = nullptr);
public:
    static Set_Ssdp *bulid(QObject *parent = nullptr);

private slots:
    void recvSlot(uchar fc, const QString &room, const QByteArray &rcv);
    void recvNumStrSlot(const sCfgItem &it);
    void recvSetSlot(const sDataItem &it);

private:
    Ssdp_Server *mSsdp;
};

#endif // SET_SSDP_H
