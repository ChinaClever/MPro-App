#ifndef KEYTHREAD_H
#define KEYTHREAD_H

#include <QObject>

class KeyThread : public QObject
{
    Q_OBJECT
public:
    explicit KeyThread(QObject *parent = nullptr);

signals:

};

#endif // KEYTHREAD_H
