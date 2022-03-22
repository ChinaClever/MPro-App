#ifndef YMODEM_H
#define YMODEM_H

#include <QObject>
#include "Ymodem/YmodemFileTransmit.h"
#include "Ymodem/YmodemFileReceive.h"

class YModem : public QObject
{
    Q_OBJECT
public:
    explicit YModem(QObject *parent = nullptr);

    bool startTransmit(const QString &name, QSerialPort *serial = nullptr);
    bool startReceive(const QString &path="", QSerialPort *serial = nullptr);
    void stop();

signals:
    void progressSig(int progress);
    void statusSig(Ymodem::Status status);

private:
    bool waitForFinish(int min=60);

private slots:
    void upStatus(Ymodem::Status status);

private:
    uchar mStatus;

    YmodemFileTransmit *mTransmit;
    YmodemFileReceive *mReceive;
};

#endif // YMODEM_H
