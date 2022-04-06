#include "pro_widget.h"
#include "ui_pro_widget.h"

Pro_Widget::Pro_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Pro_Widget)
{
    ui->setupUi(this);
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
    mTable = new ComTableWid(ui->groupBox);

//    QGridLayout *gridLayout = new QGridLayout(ui->groupBox);
//    gridLayout->setContentsMargins(0, 0, 0, 0);
//    gridLayout->addWidget(mTable);

    QStringList header;
    header << tr("目录设备") << tr("传输进度");
    mTable->initTableWid(header, 0);
}

void Pro_Widget::startSlot()
{
    mMap.clear();
    mTable->delTable();
    ui->errTextEdit->clear();
    ui->okTextEdit->clear();
}

void Pro_Widget::updateIps(const QStringList& ips)
{
    startSlot();
    for(int i=0; i<ips.size(); ++i) {
        mMap.insert(ips.at(i), i);
        mTable->setTableItem(i, 0, ips.at(i));
    }
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
    int id = mMap.value(ip);
    if(id >= 0) {
        mTable->setTableItem(id, 1, QString::number(v));
    }
}

void Pro_Widget::insertText(QPlainTextEdit *textEdit, const QString &str)
{
    textEdit->moveCursor(QTextCursor::Start);
    textEdit->insertPlainText(str+"\n");
}
