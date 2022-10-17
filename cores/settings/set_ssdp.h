#ifndef SET_SSDP_H
#define SET_SSDP_H

#include "ssdp_search.h"
#include "set_core.h"

class Set_Ssdp : public Ssdp_Obj
{
    Q_OBJECT
    explicit Set_Ssdp(QObject *parent = nullptr);
public:
    static Set_Ssdp *bulid(QObject *parent = nullptr);
    bool setting(sDataItem &it, const QString &room, const QString &ip);
    bool setCfg(sCfgItem &it, const QVariant &v, const QString &room, const QString &ip);
    bool setJson(const QByteArray &data, const QString &room, const QString &ip);

private:
    bool checkInput(const sSdpIt &it);

private slots:
    void recvSlot(const sSdpIt &it);
    void recvCfgSlot(const sCfgItem &it, const QVariant &v);
    void recvSetSlot(const sDataItem &it);
    bool rplySearchTarget(const sSdpIt &it);
};

#endif // SET_SSDP_H
