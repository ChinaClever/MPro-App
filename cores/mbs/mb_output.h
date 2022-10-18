#ifndef MB_OUTPUT_H
#define MB_OUTPUT_H
#include "mb_line.h"

class Mb_Output : public Mb_Line
{
public:
    explicit Mb_Output(QObject *parent = nullptr);

protected:
    void mbOutputUpdate();

private:
    void upOutputData();
    void upOutputThreshold();
};

#endif // MB_OUTPUT_H
