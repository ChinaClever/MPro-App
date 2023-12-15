#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "commons.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    this->setContentsMargins(0, 0, 0, 0); // 设置边距为0
//    this->setWindowFlags(Qt::FramelessWindowHint); // 设置窗口为无边框

//    cm::dataPacket();
}

MainWindow::~MainWindow()
{
    delete ui;
}

