#ifndef SET_RWOBJ_H
#define SET_RWOBJ_H

#include "cfg_obj.h"
#include "set_rwstream.h"

#define SET_DATA_FN     "set_datas.ini"
#define SET_DEFAULT_FN  "set_default.ini"

class Set_RwObj : public QObject
{
    Q_OBJECT
public:
    explicit Set_RwObj(QObject *parent = nullptr);

    void writeSettings();

public slots:
    void run() {saveSettings();}

protected:
    virtual void fillData()=0;
    virtual void unSequence()=0;
    bool readSetting(const QString &fn);
    set::_sDevData *getDev() {return mData;}

private:
    bool saveSettings();
    QByteArray toDataStream();
    set::_sDevData *deDataStream(QByteArray &array);

protected:
    CThread *mThread;

private:
    bool isRun;
    QFile *mFile;
    set::_sDevData *mData;
    Set_RwStream *mDataStream;
};

#endif // SET_RWOBJ_H
