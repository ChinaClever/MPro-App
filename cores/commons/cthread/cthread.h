#ifndef CTHREAD_H
#define CTHREAD_H
#include <QThread>
#include "crc.h"

class CThread : public QObject
{
    Q_OBJECT
public:
    explicit CThread(QObject *parent = nullptr);
    ~CThread() {stop();}

    void msleep(int msec=1);
    void init(QObject *p, const char *fun);

signals:

public slots:
    void stop();
    void finished();
    void start(bool run=true);
    void onceRun(){start(false);}

private:
    bool isSleep;
    bool isContinue;
    QThread *mThread;
};

#define END_CRC ((ushort)0x5C5C)
#endif // CTHREAD_H
