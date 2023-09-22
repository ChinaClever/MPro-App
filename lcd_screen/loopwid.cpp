#include "loopwid.h"
#include "ui_loopwid.h"

LoopWid::LoopWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoopWid)
{
    ui->setupUi(this);
}

LoopWid::~LoopWid()
{
    delete ui;
}
