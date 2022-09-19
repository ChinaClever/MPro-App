#ifndef SSDP_SEARCH_H
#define SSDP_SEARCH_H

#include "ssdp_obj.h"

class Ssdp_Search : public Ssdp_Obj
{
    Q_OBJECT
public:
    explicit Ssdp_Search(QObject *parent = nullptr);
    QStringList searchAll(){return  searchTarget("", "");}
    QStringList searchRoom(const QString &room);
    QStringList searchDev(const QString &ip);

signals:
    void targetSig(const QString &);

private:
    QStringList respondList();
    QStringList searchTarget(const QString &room, const QString &ip);

private slots:
    void searchRecvSlot(const sSdpIt &it);

private:
    QSet<QString> mSet;
};

#endif // SSDP_SEARCH_H
