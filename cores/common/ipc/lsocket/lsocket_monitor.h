#ifndef LSOCKET_MONITOR_H
#define LSOCKET_MONITOR_H

#include "dbus_receiver.h"

class LSocket_Monitor : public SM_Obj
{
    Q_OBJECT
    explicit LSocket_Monitor(QObject *parent = nullptr);
public:
    static LSocket_Monitor *bulid(QObject *parent = nullptr);
    QStringList onLines();
    QStringList offLines();

signals:
    void newConectSig(const QString &name);
    void disconectSig(const QString &name);

private:
    friend class LSocket_Server;
    QSet<QString> mOnLines, mOffLines;
};

#endif // LSOCKET_MONITOR_H
