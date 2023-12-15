#ifndef LINEWID_H
#define LINEWID_H

#include <QWidget>

namespace Ui {
class LineWid;
}

class LineWid : public QWidget
{
    Q_OBJECT

public:
    explicit LineWid(QWidget *parent = nullptr);
    ~LineWid();

private:
    Ui::LineWid *ui;
};

#endif // LINEWID_H
