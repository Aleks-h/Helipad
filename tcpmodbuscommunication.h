#ifndef TCPMODBUSCOMMUNICATION_H
#define TCPMODBUSCOMMUNICATION_H
#include <QObject>

#include <QModbusDataUnit>
#include <QModbusTcpClient>
#include <QVariant>
#include <QUrl>
#include <QThread>

#include <QDebug>
#include <QTimer>
#include <QtGlobal>


QT_BEGIN_NAMESPACE
class TCPModbusCommunication;
QT_END_NAMESPACE

class TCPModbusCommunication : public QObject
{
    Q_OBJECT

public:
    TCPModbusCommunication();
    virtual ~TCPModbusCommunication();


private:
    QModbusTcpClient *modbusDevice;
    int TimeOut;
    int numberOfRetries;
    bool answer;
    QModbusDataUnit DataUnit;


    QTimer *timer1;

    void openSocket(const QString& address = "192.168.3.18", const int& port = 502);
    void readValue();
    void readValueOnce();




public slots:
    void Connection(const QString& address = "192.168.3.18", const int& port = 502);
    void writeValue();

private slots:
    void onReadReady();


};

#endif // TCPMODBUSCOMMUNICATION_H
