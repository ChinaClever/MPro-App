/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "modbus_slavetcp.h"

Modbus_SlaveTcp::Modbus_SlaveTcp(QObject *parent) : Modbus_SlaveObj{parent}
{

}


bool Modbus_SlaveTcp::connectModbusTcp(const QString &url, int port)
{

    qDebug() << "AAAAAAAAAA" << url << port;
    setAddress(1);
    initConnects();
    initModbusNet(url, port);
    return connectDevice();
}

bool Modbus_SlaveTcp::connectTcp(int port, const QHostAddress &url)
{
    if(mDev) {
        disconnectModbus();
    } else {
        mDev = new QModbusTcpServer(this);

        initUnitMap();
        qDebug() << "AAAAAAAAA" << port;
    }
    return connectModbusTcp(url.toString(), port);
}
