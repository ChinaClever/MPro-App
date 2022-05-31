#ifndef APP_START_H
#define APP_START_H

#include <QObject>

class App_Start : public QObject
{
    Q_OBJECT
    explicit App_Start(QObject *parent = nullptr);
public:
    static App_Start *bulid(QObject *parent = nullptr);

signals:

private slots:
    void initFunSlot();
    void startThreadSlot();
    void clearCacheSlot();
};

#endif // APP_START_H
