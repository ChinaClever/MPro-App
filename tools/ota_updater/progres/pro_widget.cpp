#include "pro_widget.h"
#include "ui_pro_widget.h"

Pro_Widget::Pro_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Pro_Widget)
{
    ui->setupUi(this); mCnt = 0;
    groupBox_background_icon(this);
    QTimer::singleShot(150,this,SLOT(initFunSlot()));
//    QGridLayout *gridLayout = new QGridLayout(parent);
//    gridLayout->setContentsMargins(0, 0, 0, 0);
//    gridLayout->addWidget(this);
}

Pro_Widget::~Pro_Widget()
{
    delete ui;
}

void Pro_Widget::initFunSlot()
{
    Core_Sender *sender = Core_Sender::bulid(this);
    connect(sender, &Core_Sender::subProgress, this, &Pro_Widget::subProgressSlot);
    connect(sender, &Core_Sender::finishSig, this, &Pro_Widget::finishSlot);
}

void Pro_Widget::startSlot()
{
    mCnt = 0;
    ui->realTextEdit->clear();
}

void Pro_Widget::finishSlot(bool pass, const QString &ip)
{
    QPlainTextEdit *t = nullptr;
    if(pass) t = ui->okTextEdit;
    else t = ui->errTextEdit;
    insertText(t, ip);
}

void Pro_Widget::subProgressSlot(const QString &ip ,int v)
{
    QString str = tr("%1 \t  %2").arg(ip).arg(v);
    if(mCnt > 500) ui->realTextEdit->clear();
    insertText(ui->realTextEdit, str);
}

void Pro_Widget::insertText(QPlainTextEdit *textEdit, const QString &str)
{
    textEdit->moveCursor(QTextCursor::Start);
    textEdit->insertPlainText(str+"\n");
}
