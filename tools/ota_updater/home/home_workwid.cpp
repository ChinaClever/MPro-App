/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "home_workwid.h"
#include "ui_home_workwid.h"
#include <QFileDialog>
#include "msgbox.h"

Home_WorkWid::Home_WorkWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home_WorkWid)
{
    ui->setupUi(this);
    QTimer::singleShot(5,this,SLOT(initFunSlot()));
}

Home_WorkWid::~Home_WorkWid()
{
    delete ui;
}

void Home_WorkWid::initLayout()
{
    QPalette pl = ui->textEdit->palette();
    pl.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));
    ui->textEdit->setPalette(pl);
    //ui->userEdit->setPalette(pl); //ui->cntSpin->setPalette(pl);
    //ui->textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //ui->textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QGridLayout *gridLayout = new QGridLayout(this->parentWidget());
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
}

void Home_WorkWid::initFunSlot()
{
    initLayout(); isStart = false;
    mSender = Core_Sender::bulid(this);
    connect(mSender, &Core_Sender::infoMessage, this, &Home_WorkWid::insertTextSlot);
    connect(mSender, &Core_Sender::finishSig, this, &Home_WorkWid::finishSlot);
    connect(mSender, &Core_Sender::overSig, this, &Home_WorkWid::updateResult);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    //    QTimer::singleShot(450,this,SLOT(updateCntSlot()));
}

void Home_WorkWid::setTextColor(bool pass)
{
    QColor color("black");
    if(!pass) {color = QColor("red"); mResult = pass;}
    ui->textEdit->moveCursor(QTextCursor::Start);

    QTextCharFormat fmt;//文本字符格式
    fmt.setForeground(color);// 前景色(即字体色)设为color色
    QTextCursor cursor = ui->textEdit->textCursor();//获取文本光标
    cursor.mergeCharFormat(fmt);//光标后的文字就用该格式显示
    ui->textEdit->mergeCurrentCharFormat(fmt);//textEdit使用当前的字符格式
}

void Home_WorkWid::insertTextSlot(bool pass, const QString &msg)
{
    setTextColor(pass);
    QString str = QString::number(mId++) + "、"+ msg + "\n";
    ui->textEdit->insertPlainText(str);
    if(mId > 500) ui->textEdit->clear();
}

void Home_WorkWid::finishSlot(bool pass, const QString &msg)
{
    QString str;
    if(pass) str = tr(" 文件上传成功！");
    else str = tr(" 文件上传失败！");
    insertTextSlot(pass, msg+str);
}

void Home_WorkWid::updateCntSlot()
{
    //    sCount *cnt = &(mItem->cnts);
    //    ui->okLcd->display(cnt->ok);
    //    ui->allLcd->display(cnt->all);
    //    ui->errLcd->display(cnt->err);

    //    ui->cntSpin->setValue(mItem->cnts.cnt);
    //    if(mItem->cnts.cnt < 1)mItem->user.clear();
    //    ui->userEdit->setText(mItem->user);

    //    QString str = "0";
    //    if(cnt->all) {
    //        double value = cnt->ok*100.0 / cnt->all;
    //        str = QString::number(value,'f',0) +"%";
    //    }
    //    ui->passLcd->display(str);
}

QString Home_WorkWid::getTime()
{
    QTime t(0,0,0,0);
    t = t.addSecs(startTime.secsTo(QTime::currentTime()));
    return  tr("%1").arg(t.toString("mm:ss"));
}

void Home_WorkWid::updateTime()
{
    QString str = getTime();
    QString style = "background-color:yellow; color:rgb(0, 0, 0);";
    style += "font:100 34pt \"微软雅黑\";";

    ui->timeLab->setText(str);
    ui->timeLab->setStyleSheet(style);
}

void Home_WorkWid::setWidEnabled(bool en)
{
    ui->modeBox->setEnabled(en);
    ui->groupBox_2->setEnabled(en);
}

void Home_WorkWid::updateResult()
{
    QString style;
    QString str = tr("---");
    if(mResult) {
        str = tr("成功"); style = "background-color:green; color:rgb(255, 255, 255);";
    } else {
        str = tr("失败"); style = "background-color:red; color:rgb(255, 255, 255);";
    } style += "font:100 34pt \"微软雅黑\";";

    isStart = false;
    setWidEnabled(true);
    ui->timeLab->setText(str);
    ui->timeLab->setStyleSheet(style);
    ui->startBtn->setText(tr("开 始"));
    QTimer::singleShot(450,this,SLOT(updateCntSlot()));
    str = QTime::currentTime().toString("hh:mm:ss");
    ui->endLab->setText(str);
    mSender->stopRun();
    timer->stop();
}


void Home_WorkWid::timeoutDone()
{
    updateTime();
}


void Home_WorkWid::initData(sFileTrans &it)
{
    mId = 1; mResult = true;
    it.file = ui->fnLab->text().split("/").last();
    it.md5 = File::Md5(ui->fnLab->text());
    it.fc = ui->modeBox->currentIndex();
    it.dev = ui->devBox->currentText();
    it.path = ui->pathEdit->text();
}

bool Home_WorkWid::inputCheck()
{
    ui->textEdit->clear();
    QString str = ui->pathEdit->text();
    if(str.isEmpty()) {MsgBox::critical(this, tr("接收路径，未指定。")); return false;}
    else if(str.right(1) != "/") ui->pathEdit->setText(str + "/");

    return true;
}

bool Home_WorkWid::initWid()
{
    bool ret = inputCheck();
    if(ret) {
        initData(mFileIt);
        setWidEnabled(false);
        ui->startBtn->setText(tr("终 止"));
        startTime = QTime::currentTime(); emit startSig();
        QString str = startTime.toString("hh:mm:ss");
        ui->startLab->setText(str);
        ui->endLab->setText("---");
        isStart = true;
    }

    return ret;
}

void Home_WorkWid::on_startBtn_clicked()
{
    if(isStart == false) {
        if(initWid()) {
            QStringList ips;
            ips << "127.0.0.1";
            timer->start(500);
            mSender->sendFile(ips, ui->fnLab->text(), mFileIt);
        }
    } else {
        bool ret = MsgBox::question(this, tr("确定需要提前结束？"));
        if(ret) { mResult = false; updateResult(); }
    }
}

bool Home_WorkWid::fileCrc(const QString &fn)
{
    bool ret = false;
    int index = ui->devBox->currentIndex();
    switch (index) {
    case 1: ret = File::CheckMd5(fn); break;
    case 2: ret = File::CheckCrc(fn); break;
    default: MsgBox::critical(this, tr("请选择需要选择的设备类型")); return ret;
    }

    if(!ret) MsgBox::critical(this, tr("你选择的文件，未能通过检验。"));
    return ret;
}


void Home_WorkWid::on_imgBtn_clicked()
{
    QString str = tr("请选择");  QString dir = QDir::home().absolutePath();
    QString fn = QFileDialog::getOpenFileName(this, tr("选择烧录文件"), dir, "镜像文件(*.*)");
    if(fn.isEmpty()) return;

    bool en = fileCrc(fn);
    if(en){
        str = tr("已选择");
        ui->fnLab->setText(fn);
    } else ui->fnLab->clear();

    ui->imgBtn->setText(str);
    ui->startBtn->setEnabled(en);
}



