#ifndef LOOPWID_H
#define LOOPWID_H

#include <QWidget>

namespace Ui {
class LoopWid;
}

class LoopWid : public QWidget
{
    Q_OBJECT

public:
    explicit LoopWid(QWidget *parent = nullptr);
    ~LoopWid();

private:
    Ui::LoopWid *ui;
};

#endif // LOOPWID_H
