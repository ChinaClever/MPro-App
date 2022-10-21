#ifndef OTASSDP_H
#define OTASSDP_H

#include "ssdp_search.h"

class OtaSsdp : public Ssdp_Obj
{
    Q_OBJECT
public:
    explicit OtaSsdp(QObject *parent = nullptr);

private:
    bool checkInput(const sSdpIt &it);
    void inet_updateInterface();

private slots:
     bool rplySearchTarget(const sSdpIt &it);
};

#endif // OTASSDP_H
