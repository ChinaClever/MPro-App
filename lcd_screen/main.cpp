#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

//./lcd_screen -platform linuxfb

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFontDatabase::addApplicationFont("/usr/data/pdu/fonts/roboto/Roboto-Medium.ttf");


    MainWindow w;
    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsProxyWidget *gpw = new QGraphicsProxyWidget();
    gpw->setWidget(&w); scene->addItem(gpw);
    gpw->setRotation(180); // 旋转 180 度

    QGraphicsView *view = new QGraphicsView(scene);
    view->setFrameStyle(QFrame::NoFrame); // 设置视图的框架样式为无框架
    view->showFullScreen();//全屏显示

    return a.exec();
}
