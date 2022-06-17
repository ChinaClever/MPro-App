#ifndef CFG_RWOBJ_H
#define CFG_RWOBJ_H

#include "cfg_obj.h"
#include "cfg_rwstream.h"

#define CFG_DATA_FN     "cfg_datas.ini"
#define CFG_DEFAULT_FN  "cfg_default.ini"

class Cfg_RwObj : public QObject
{
    Q_OBJECT
public:
    explicit Cfg_RwObj(QObject *parent = nullptr);

    void writeSettings();

public slots:
    void run() {saveSettings();}

protected:
    virtual void fillData()=0;
    virtual void unSequence()=0;
    bool readSetting(const QString &fn);
    cfg::_sDevData *getDev() {return mData;}

private:
    bool saveSettings();
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
