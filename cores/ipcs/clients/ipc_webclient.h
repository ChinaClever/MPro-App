#ifndef IPC_WEBCLIENT_H
#define IPC_WEBCLIENT_H

#include "ipc_logclient.h"

class IPC_WebClient : public IPC_LogClient
{
    Q_OBJECT
    explicit IPC_WebClient(QObject *parent = nullptr);
public:
    static IPC_WebClient *bulid(QObject *parent = nullptr);
    bool upload(const QString &fn,const QString &md5="");
    QString getString(uchar addr, uchar fc, uchar id);
    bool setString(uint addr, uchar fc, uchar id, const QString &str);

    int getDevCfg(uchar addr, uchar fc, uchar type);
    bool setDevCfg(uint addr, uchar fc, uchar type, int value);

    int getValue(uchar addr, uchar type, uchar topic, uchar sub, uchar id);
    bool setting(uint addr, uchar type, uchar topic, uchar sub, uchar id, uint value);

protected:
    bool multipleStrings(uchar fc, const QString &key, const QString &value);

private:
    bool getValue(sDataItem &unit);
    QString getNumStr(sNumStrItem &unit);
    bool setting(const sDataItem &unit);
    bool setNumStr(const sNumStrItem &unit);
    bool msgSend(int fc, const QByteArray &msg);
};

#endif // IPC_WEBCLIENT_H
