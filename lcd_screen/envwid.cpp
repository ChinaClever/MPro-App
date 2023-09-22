#include "envwid.h"
#include "ui_envwid.h"

EnvWid::EnvWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnvWid)
{
    ui->setupUi(this);
}

EnvWid::~EnvWid()
{
    delete ui;
}
