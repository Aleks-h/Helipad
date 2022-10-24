#include "tcpmodbuscommunication.h"

TCPModbusCommunication::TCPModbusCommunication() : modbusDevice (nullptr),
                                                   TimeOut (1000),
                                                   numberOfRetries(3)

{

}

TCPModbusCommunication::~TCPModbusCommunication()
{
    if (modbusDevice)
        modbusDevice->disconnectDevice();
    delete modbusDevice;
    delete timer1;
}

void TCPModbusCommunication::Connection(const QString& address, const int& port)
{
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
        for (qsizetype i = 0, total = unit.valueCount(); i < total; ++i) {
            const QString entry = tr("Address: %1, Value: %2").arg(unit.startAddress() + i)
                                     .arg(QString::number(unit.value(i),
                                          unit.registerType() <= QModbusDataUnit::Coils ? 10 : 16));
            qDebug() << entry;
        }
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
