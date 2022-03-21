#ifndef LSOCKET_OBJECT_H
#define LSOCKET_OBJECT_H

#include "sm_obj.h"

class LSocket_Object : public SM_Obj
{
    Q_OBJECT
public:
    explicit LSocket_Object(QObject *parent = nullptr);
    virtual void setKey(const QString &newKey);
    const QString &key() const;
protected:
    QString mKeyObj;
};

#endif // LSOCKET_OBJECT_H
