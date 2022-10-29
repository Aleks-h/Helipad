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
#include <QSqlQuery>
#include <QString>
#include <QMessageBox>
#include <QSound>


QT_BEGIN_NAMESPACE
class TCPModbusCommunication;
QT_END_NAMESPACE

class TCPModbusCommunication : public QObject
{
    Q_OBJECT

public:
    TCPModbusCommunication(QVector <bool>& CurantState);
    virtual ~TCPModbusCommunication();

    QVector<quint16> Value;
    QVector<bool> &CurState;


private:
    QModbusTcpClient *modbusDevice;
    int TimeOut;
    int numberOfRetries;

    QString address;
    QString port;

    QTimer *timer1;


    void openSocket(const QString& address = "192.168.3.18", const int& port = 502);
    void readValue();
    void readValueOnce();

    void errorCheck();
    QString error;
    QString curErr;


signals:
    void logging(QString Data);

public slots:
    void Connection(const QString& address = "192.168.3.18", const int& port = 502);
    void writeValue(int subsystemNmbr, bool ReqState);

private slots:
    void onReadReady();

};

#endif // TCPMODBUSCOMMUNICATION_H
