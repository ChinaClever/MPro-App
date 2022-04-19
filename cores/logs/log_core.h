#ifndef LOG_CORE_H
#define LOG_CORE_H

#include "db_sys.h"

class Log_Core : public QObject
{
    Q_OBJECT
    explicit Log_Core(QObject *parent = nullptr);
public:
    static Log_Core *bulid(QObject *parent = nullptr);
    void append(const sOpItem &it) {mOpIts<<it; run();}
    void append(const sSysItem &it) {mSysIts<<it; run();}
    void append(const sUserItem &it) {mUserIts<<it; run();}

private slots:
    void run();
    void initFunSlot();
    void saveLogSlot();

private:
    bool isRun;
    Db_Op *mOp;
    QList<sOpItem> mOpIts;

    Db_User *mUser;
    QList<sUserItem> mUserIts;

    Db_Sys *mSys;
    QList<sSysItem> mSysIts;
};

#endif // LOG_CORE_H
