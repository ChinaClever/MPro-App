#ifndef HOME_WORKWID_H
#define HOME_WORKWID_H
#include "file.h"
#include "home_statewid.h"
#include "core_sender.h"

namespace Ui {
class Home_WorkWid;
}

class Home_WorkWid : public QWidget
{
    Q_OBJECT

public:
    explicit Home_WorkWid(QWidget *parent = nullptr);
    ~Home_WorkWid();

signals:
    void startSig();

protected:
    void initLayout();
    QString getTime();
    void updateTime();
    void setWidEnabled(bool en);
    void setTextColor(bool pass);

    bool initWid();
    bool inputCheck();
    void initData(sFileTrans &it);
    bool fileCrc(const QString &fn);

private slots:
    void timeoutDone();
    void initFunSlot();    
    void updateResult();

    void updateCntSlot();
    void on_startBtn_clicked();    
    void on_imgBtn_clicked();
    void insertTextSlot(bool pass, const QString &msg);
    void finishSlot(bool pass, const QString &msg);

private:
    Ui::Home_WorkWid *ui;

    int mId;
    bool isStart;
    bool mResult;
    QTimer *timer;
    sFileTrans mFileIt;
    Core_Sender *mSender;

    QTime startTime;
//    Test_CoreThread *mCoreThread;
};

#endif // HOME_WORKWID_H
