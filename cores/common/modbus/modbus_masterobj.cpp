#include "modbus_masterobj.h"

Modbus_MasterObj::Modbus_MasterObj(QObject *parent) : Modbus_Object{parent}
{
    mModbus = nullptr;
}

void Modbus_MasterObj::initConnects()
{
    connect(mModbus, &QModbusClient::stateChanged, this, &Modbus_MasterObj::onStateChanged);
    connect(mModbus, &QModbusClient::errorOccurred, this, &Modbus_MasterObj::handleDeviceError);
}

QVector<quint16> Modbus_MasterObj::readModbus(int addr, const QModbusDataUnit &unit)
{
    QVector<quint16> data; if(!isConnectedModbus()) return data;
    if(auto *reply = mModbus->sendReadRequest(unit, addr)) {
        if(replyFinished(reply)) data = reply->result().values();
        reply->deleteLater();
    } else throwError(Q_FUNC_INFO);
    return data;
}

QVector<quint16> Modbus_MasterObj::readModbus(int addr, QModbusDataUnit::RegisterType table, int start, int num)
{
    QModbusDataUnit unit(table, start, num);
    return readModbus(addr, unit);
}

QVector<quint16> Modbus_MasterObj::readModbus(int addr, int start, int num)
{
    return readModbus(addr, m_type, start, num);
}

void Modbus_MasterObj::writeRequest(const QModbusDataUnit &dataunit, int serverAddress)
{
    if(auto *reply = mModbus->sendWriteRequest(dataunit,  serverAddress)) {
        connect(reply, &QModbusReply::finished, this, [reply](){
            QModbusPdu::ExceptionCode code = reply->rawResult().exceptionCode();
            if(reply->error() == QModbusDevice::ProtocolError){
                qDebug() << tr("Write response error:%1 (modbus exception:0x%2)")
                            .arg(reply->errorString()).arg(code, -1, 16);
            }else{
                qDebug() << tr("Write response:%1 (code:0x%2)")
                            .arg(reply->errorString()).arg(code, -1, 16);
            }
            reply->deleteLater();
        });
    } else throwError(tr("Write error:"));
}


int Modbus_MasterObj::writeModbus(int addr, const QModbusDataUnit &unit)
{
    int ret = -1; if(!isConnectedModbus()) return ret;
    if(auto *reply = mModbus->sendWriteRequest(unit, addr)) {
        if(replyFinished(reply)) ret = reply->rawResult().exceptionCode();
        reply->deleteLater(); } else throwError(Q_FUNC_INFO);
    return ret;
}

int Modbus_MasterObj::writeModbus(int addr, QModbusDataUnit::RegisterType table, int start, const QVector<quint16> &data)
{
    QModbusDataUnit dataunit(table, start, data);
    return writeModbus(addr, dataunit);
}

int Modbus_MasterObj::writeModbus(int addr, int start, const QVector<quint16> &data)
{
    return writeModbus(addr, m_type, start, data);
}
