#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H
#include <QtCore>

class Serial_Port
{
public:
    Serial_Port();
    bool OpenPort(const char * dev);
    int setup(int speed,int flow_ctrl,int databits,int stopbits,int parity);
    //int set_Parity(int databits,int stopbits,int parity);
    //void setBaudRate(int speed);

    int readBuffer(uint8_t * buffer,int size);
    int writeBuffer(uint8_t * buffer,int size);
    uint8_t getchar();
    void closePort();
    bool isOpen();

    void flush();
    bool isReadable() {return isOpen();}
    bool isWritable() {return isOpen();}
    QString portName() {return m_portName;}
    void setPortName(const QString &name) {m_portName = name;}
    bool openPort() {return OpenPort(m_portName.toLocal8Bit().data());}
    int writePort(const QByteArray &array);
    QByteArray readAll();

private:
    int fd;
    QString m_portName;
    uint8_t mBuf[1024*20];
};

#endif // SERIAL_PORT_H
