#ifndef APP_START_H
#define APP_START_H

#include <QtCore>

class App_Start : public QObject
{
    Q_OBJECT
    explicit App_Start(QObject *parent = nullptr);
public:
    static App_Start *bulid(QObject *parent = nullptr);
    ~App_Start();

private:
    void compileTime();

private slots:
    void initFunSlot();
    void startThreadSlot();


};

#endif // APP_START_H
