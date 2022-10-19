#ifndef TCPMODBUSCOMMUNICATION_H
#define TCPMODBUSCOMMUNICATION_H
#include <QObject>

#include <QModbusDataUnit>
#include <QModbusTcpClient>
#include <QVariant>

QT_BEGIN_NAMESPACE

class QModbusClient;
class QModbusReply;

namespace Ui {
class MainWindow;
class SettingsDialog;
}
QT_END_NAMESPACE

class TCPModbusCommunication : public QObject
{
    Q_OBJECT
public:
    TCPModbusCommunication();
    virtual ~TCPModbusCommunication();

    void Connection();

private:
    QModbusTcpClient *modbusDevice;

};

#endif // TCPMODBUSCOMMUNICATION_H
