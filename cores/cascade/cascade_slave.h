#ifndef CASCADE_SLAVE_H
#define CASCADE_SLAVE_H

#include "cascade_fill.h"

class Cascade_Slave : public Cascade_Fill
{
    Q_OBJECT
    explicit Cascade_Slave(QObject *parent = nullptr);
public:
    static Cascade_Slave *bulid(QObject *parent = nullptr);
    ~Cascade_Slave();

public slots:
    void run();
    void start();

private:
    bool replyDevData(uchar fc);
    bool replyAlarm(QByteArray &rcv);
    bool replyRelaySet(QByteArray &rcv);
    bool replyRelayCtrl(QByteArray &rcv);
    bool workDown(c_sFrame &it);

private:
    bool isRun;
    QFile *mFile;
    CThread *mThread;
};

using Cascade_Core = Cascade_Slave;

#endif // CASCADE_SLAVE_H
