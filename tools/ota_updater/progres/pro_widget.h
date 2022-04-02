#ifndef PRO_WIDGET_H
#define PRO_WIDGET_H

#include "home_mainwid.h"
#include <QPlainTextEdit>

namespace Ui {
class Pro_Widget;
}

class Pro_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Pro_Widget(QWidget *parent = nullptr);
    ~Pro_Widget();

public slots:
    void startSlot();

private slots:
    void initFunSlot();
    void finishSlot(bool, const QString &);
    void subProgressSlot(const QString &ip, int);
    void insertText(QPlainTextEdit *textEdit, const QString &str);

private:
    Ui::Pro_Widget *ui;
    int mCnt;
};

#endif // PRO_WIDGET_H
