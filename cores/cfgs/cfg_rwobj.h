#ifndef CFG_RWOBJ_H
#define CFG_RWOBJ_H

#include "cfg_obj.h"
#include "cfg_rwstream.h"

#define CFG_ALARM_FN    "cfg_alarm.bin"
#define CFG_ALARM_DF    "cfg_alarm.df"
#define CFG_PARAM_FN    "cfg_param.bin"
#define CFG_PARAM_DF    "cfg_param.df"


class Cfg_RwObj : public QObject
{
    Q_OBJECT
public:
    explicit Cfg_RwObj(QObject *parent = nullptr);

    bool writeParams();
    void writeAlarms();

public slots:
    void run() {saveAlarms();}

protected:
    virtual void fillData()=0;
    virtual void unSequence()=0;
    bool readParam(const QString &fn);
    bool readAlarm(const QString &fn);
    cfg::_sDevData *getDev() {return mData;}

private:
    bool saveAlarms();
    QByteArray toDataStream();
    cfg::_sDevData *deDataStream(QByteArray &array);

protected:
    CThread *mThread;

private:
    bool isRun;
    QFile *mFile;
    cfg::_sDevData *mData;
    Cfg_RwStream *mDataStream;
};

#endif // CFG_RWOBJ_H
