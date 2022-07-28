#ifndef WEBRPCOBJ_H
#define WEBRPCOBJ_H
#include <QtCore>
#include "mongoose.h"
#include "commons.h"
#include "ipc_webclient.h"

class WebRpcObj
{
public:
    WebRpcObj();
    double getNumber(mg_str &r, int id);
    QVector<double> getNumbers(mg_str &r, int num);
    QString getString(mg_str &r, int id);

private:
     void compileTime();
};

#endif // WEBRPCOBJ_H
