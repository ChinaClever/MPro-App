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
    bool isRun(){return mThread->isRunning();}

signals:

public slots:
    void stop();
    void finished();
    void start(bool run=true);
    void onceRun(){start(false);}
    void quit() {mThread->quit();}

private:
    bool isSleep;
    bool isContinue;
    QThread *mThread;
};

#define START_HEAD ((ushort)0x7E7E)
#define END_CRC ((ushort)0x5C5C)
#endif // CTHREAD_H
