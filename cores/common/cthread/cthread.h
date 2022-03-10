#ifndef CTHREAD_H
#define CTHREAD_H
#include <QtCore>
#include <QThread>

void mdelay(int msec=1);
class CThread : public QObject
{
    Q_OBJECT
public:
    explicit CThread(QObject *parent = nullptr);
    ~CThread() {stop();}

    void stop();
    void msleep(int msec=1);
    void start(bool run=true);
    void onceRun(){start(false);}
    void init(QObject *p, const char *fun);
signals:

protected slots:
    void finished();

private:
    bool isSleep;
    bool isContinue;
    QThread *mThread;
};

#endif // CTHREAD_H
