#ifndef ENVWID_H
#define ENVWID_H

#include <QWidget>

namespace Ui {
class EnvWid;
}

class EnvWid : public QWidget
{
    Q_OBJECT

public:
    explicit EnvWid(QWidget *parent = nullptr);
    ~EnvWid();

private:
    Ui::EnvWid *ui;
};

#endif // ENVWID_H
