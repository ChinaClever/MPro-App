#ifndef SET_OBJECT_H
#define SET_OBJECT_H

#include "set_stream.h"

class Set_Object : public QObject
{
    Q_OBJECT
public:
    explicit Set_Object(QObject *parent = nullptr);

    void writeSettings();

public slots:
    void run() {saveSettings();}
    bool readSettings();

protected:
    virtual void fillData()=0;
    virtual void unSequence()=0;
    set::_sDevData *getDev() {return mData;}

private:
    bool saveSettings();
    QByteArray toDataStream();
    set::_sDevData *deDataStream(QByteArray &array);

private:
    bool isRun;
    QFile *mFile;
    CThread *mThread;
    set::_sDevData *mData;
    Set_Stream *mDataStream;
};

#endif // SET_OBJECT_H
