#ifndef LOG_CORE_H
#define LOG_CORE_H

#include "db_op.h"
#include "cthread.h"

class Log_Core : public QObject
{
    Q_OBJECT
    explicit Log_Core(QObject *parent = nullptr);
public:
    static Log_Core *bulid(QObject *parent = nullptr);
    void append(const sOpItem &it) {mOpIts<<it; writeLogs();}
    void append(const sUserItem &it) {mUserIts<<it; writeLogs();}
    void writeLogs();


public slots:
    void run() {saveLogs();}
    void initFunSlot();

private:
    void saveLogs();

private:
    bool isRun;
    CThread *mThread;

    Db_Op *mOp;
    QList<sOpItem> mOpIts;

    Db_User *mUser;
    QList<sUserItem> mUserIts;
};

#endif // LOG_CORE_H
