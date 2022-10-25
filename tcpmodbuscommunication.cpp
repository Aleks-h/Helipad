#include "tcpmodbuscommunication.h"

TCPModbusCommunication::TCPModbusCommunication(bool &CurState) : Value{QVector<quint16>{0}},
                                                   modbusDevice {new QModbusTcpClient(this)},
                                                   TimeOut {1000},
                                                   numberOfRetries{3}

{
 this -> CurState = CurState;
}

TCPModbusCommunication::~TCPModbusCommunication()
{
    if (modbusDevice)
        modbusDevice->disconnectDevice();
    delete modbusDevice;
    delete timer1;
}

void TCPModbusCommunication::Connection(bool& CurrentState, const QString& address, const int& port)
{
  CurState = CurrentState;
  openSocket(address, port);
  readValue();
}

void TCPModbusCommunication::openSocket(const QString& address, const int& port)
{
    if (modbusDevice == nullptr)
       {
        modbusDevice = new QModbusTcpClient(this);
       }

    modbusDevice->setConnectionParameter
            (QModbusDevice::NetworkAddressParameter, address);
    modbusDevice->setConnectionParameter
            (QModbusDevice::NetworkPortParameter, port);

    modbusDevice->setTimeout(1000);
    modbusDevice->setNumberOfRetries(3);
    modbusDevice->connectDevice();
}

void TCPModbusCommunication::readValue()
{
    timer1 = new QTimer(this);
    connect (timer1, &QTimer::timeout, [=](){
                                             readValueOnce();
                                            });
    timer1->start(1000);
}

void TCPModbusCommunication::readValueOnce()
{
if (!modbusDevice)
{
    qDebug() << "Object doesn't exsist;" ;
    return;
}

if (modbusDevice->state() != QModbusDevice::ConnectedState)
   {
    qDebug() << "Connection fault" ;
    return;
   }

QModbusDataUnit DataUnit (QModbusDataUnit::Coils, 0, 1);
if (auto * reply = modbusDevice->sendReadRequest(DataUnit, 1))
    {
    connect(reply, &QModbusReply::finished, this, &TCPModbusCommunication::onReadReady);
    }
else
    {
    qDebug() << "Read error: " + modbusDevice->errorString();
}
}


void TCPModbusCommunication::onReadReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        CurState = unit.value(0);

       } else if (reply->error() == QModbusDevice::ProtocolError) {
       qDebug() << "Read response error: %1 (Modbus exception: 0x%2) "+
                                    reply->errorString()+" "+
                                    reply->rawResult().exceptionCode();
    } else {
        qDebug() << "Read response error: %1 (code: 0x%2) " +
                                    reply->errorString() + " " +
                                    reply->error();
    }

    reply->deleteLater();
}

void TCPModbusCommunication::writeValue(const int &ReqState)
{
    if (!modbusDevice){return;}
    Value[0] = ReqState;
    QModbusDataUnit DataUnit (QModbusDataUnit::Coils, 0, Value);
    if (auto *reply = modbusDevice->sendWriteRequest(DataUnit, 1)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    qDebug() << "Write response error:" + reply->errorString() +
                                "Modbus exception:" + reply->rawResult().exceptionCode();
                } else if (reply->error() != QModbusDevice::NoError) {
                    qDebug() << "Write response error:" + reply->errorString()
                                + " (code: " + reply->error();
                }
                reply->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }
    } else {
            qDebug() << "Write error: " + modbusDevice->errorString();
    }
}

