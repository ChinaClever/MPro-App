#ifndef TD_MAINWID_H
#define TD_MAINWID_H

#include "comtablewid.h"
#include "core_sender.h"

namespace Ui {
class Td_MainWid;
}

class Td_MainWid : public QWidget
{
    Q_OBJECT

public:
    explicit Td_MainWid(QWidget *parent = nullptr);
    ~Td_MainWid();

public slots:
    void startSlot();

private slots:
    void initFunSlot();
    void insertSlot(const QString &str);

private:
    Ui::Td_MainWid *ui;
    int mCnt;
};

#endif // TD_MAINWID_H
