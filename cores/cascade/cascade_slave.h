#ifndef CASCADE_SLAVE_H
#define CASCADE_SLAVE_H

#include "cascade_fill.h"

class Cascade_Slave : public Cascade_Fill
{
    Q_OBJECT
public:
    explicit Cascade_Slave(QObject *parent = nullptr);

protected:
    bool workDown(c_sFrame &it);

private:
    bool replyDevData(uchar fc);
    bool replySet(QByteArray &rcv);
    bool replyNumStr(QByteArray &rcv);
};

#endif // CASCADE_SLAVE_H
