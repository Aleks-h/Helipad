#include "tcpmodbuscommunication.h"

TCPModbusCommunication::TCPModbusCommunication()
{
  Connection();

}

void TCPModbusCommunication::Connection()
{
  if (modbusDevice == nullptr)
     {
      modbusDevice = new QModbusTcpClient(this);
     }

  if (modbusDevice->state() == QModbusDevice::ConnectedState)
     {
      return;
     }

  modbusDevice->setConnectionParameter
          (QModbusDevice::SerialPortNameParameter, 502);
  modbusDevice->setConnectionParameter
          (QModbusDevice::NetworkPortParameter, 1);
  modbusDevice->setConnectionParameter
          (QModbusDevice::NetworkAddressParameter, 2);


}
