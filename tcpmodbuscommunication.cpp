#include "tcpmodbuscommunication.h"

TCPModbusCommunication::TCPModbusCommunication(QVector<bool> &CurantState) :
                                                    Value{QVector<quint16>{0}},
                                                    CurState{CurantState},
                                                    modbusDevice {new QModbusTcpClient(this)},
                                                    TimeOut {1000},
                                                    numberOfRetries{3},
                                                    error{" "},curErr{" "}


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
    this -> address = address;
    this -> port = port;

    modbusDevice->setConnectionParameter
            (QModbusDevice::NetworkAddressParameter, address);
    modbusDevice->setConnectionParameter
            (QModbusDevice::NetworkPortParameter, port);

    modbusDevice->setTimeout(TimeOut);
    modbusDevice->setNumberOfRetries(numberOfRetries);
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
    error = "Модбас, не удалось установить соеденение с хостом " + address;
    errorCheck();
    return;
   }

QModbusDataUnit DataUnit (QModbusDataUnit::Coils, 0, CurState.size());
if (auto * reply = modbusDevice->sendReadRequest(DataUnit, 1))
    {
    connect(reply, &QModbusReply::finished, this, &TCPModbusCommunication::onReadReady);
    }
else
    {
    error = "Модбас, ошибка чтения: " + modbusDevice->errorString();
    errorCheck();
}
   // if(modbusDevice->errorString() == 0)
    //{
   //     error = "Связь с устройством восстановлена";
   //     errorCheck();
    //}
}

void TCPModbusCommunication::errorCheck()
{
    if (error != curErr)
    {
        curErr=error;
        logging(error);


    }

}



void TCPModbusCommunication::onReadReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();

        error = "Модбас, соеденение установлено";
        errorCheck();

        for (int i = 0, total = unit.valueCount(); i < total; ++i)
        {
        CurState[i] = unit.value(i);
        }


       } else if (reply->error() == QModbusDevice::ProtocolError) {

       error = "Модбас, чтение, ошибка ответа: " + reply->errorString();
       errorCheck();


    } else {
        error = "Модбас, чтение, ошибка ответа: " + reply->errorString();
        errorCheck();
    }

    reply->deleteLater();
}

void TCPModbusCommunication::writeValue(int subsystemNmbr, bool ReqState)
{
    Value[0]=ReqState;
    QModbusDataUnit DataUnit (QModbusDataUnit::Coils, subsystemNmbr, Value);
    if (auto *reply = modbusDevice->sendWriteRequest(DataUnit, 1)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [=]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                   error = "Модбас, запись, ошибка ответа: " + reply->errorString();
                   errorCheck();
                } else if (reply->error() != QModbusDevice::NoError) {
                   error = "Модбас, запись, ошибка ответа: " + reply->errorString();
                   errorCheck();
                }
                reply->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }
    } else {
            error = "Модбас, ошибка записи: " + modbusDevice->errorString();
            errorCheck();
    }
}

