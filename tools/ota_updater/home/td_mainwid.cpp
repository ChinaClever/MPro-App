/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "td_mainwid.h"
#include "ui_td_mainwid.h"


Td_MainWid::Td_MainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Td_MainWid)
{
    ui->setupUi(this);  mCnt = 0;
    QTimer::singleShot(1350,this,SLOT(initFunSlot()));
    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
}

Td_MainWid::~Td_MainWid()
{
    delete ui;
}

void Td_MainWid::initFunSlot()
{
    Core_Sender *sender = Core_Sender::bulid(this);
    connect(sender, &Core_Sender::throwSig, this, &Td_MainWid::insertSlot);
    connect(sender, &Core_Sender::errorMessage, this, &Td_MainWid::insertSlot);
}

void Td_MainWid::startSlot()
{
    ui->textEdit->clear();  mCnt = 0;
}

void Td_MainWid::insertSlot(const QString &str)
{
    if(str.size()>2) ui->textEdit->appendPlainText(str);
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(cursor);
    if(mCnt++ > 500) startSlot();
}


