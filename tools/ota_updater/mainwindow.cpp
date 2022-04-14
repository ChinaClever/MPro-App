#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mNavBarWid = new NavBarWid(ui->barWid);
    QTimer::singleShot(50,this,SLOT(initFunSlot())); //延时初始化
    connect(mNavBarWid, SIGNAL(navBarSig(int)), this, SLOT(navBarSlot(int)));
    qDebug() << "QSslSocket=" << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "OpenSSL支持情况:" << QSslSocket::supportsSsl();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initFunSlot()
{
    mHomeWid = new Home_MainWid(ui->stackedWid);
    ui->stackedWid->addWidget(mHomeWid);

    mPro = new Pro_Widget(ui->stackedWid);
    ui->stackedWid->addWidget(mPro);
    connect(mHomeWid, SIGNAL(startSig()), mPro, SLOT(startSlot()));

    mRecvWid = new Remote_RecvWid(ui->stackedWid);
    ui->stackedWid->addWidget(mRecvWid);
    connect(mHomeWid, SIGNAL(startSig()), mRecvWid, SLOT(startSlot()));
}

void MainWindow::navBarSlot(int id)
{
    ui->stackedWid->setCurrentIndex(id);
}
