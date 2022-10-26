#ifndef QIMAGEWIDGET_H
#define QIMAGEWIDGET_H
#include <QWidget>
#include <QPixmap>
#include "QPainter"
#include <QMessageBox>
#include <QSound>
#include <QThread>
#include <memory>

#include "tcpmodbuscommunication.h"

class QImageWidget : public QWidget
{
Q_OBJECT
public:
explicit QImageWidget (QString);
virtual~QImageWidget();
QString name;
void state(const int);
void setPixmap(QPixmap pixmap);


private:
QPixmap _originalImage;
int cur_picture_prev;
void updateMargins();
int cur_picture;
bool ISalarm = 0;

void button_on_pushed();
void button_off_pushed();

bool button_ON_pushed;
bool button_OFF_pushed;

QTimer* timer;
QTimer* timer1;
QTimer* timer2 = nullptr;
QTimer* timer3 = nullptr;
QTimer* timer4;
QTimer* timer5;

static int counter;
QString address;

TCPModbusCommunication *TCPModbus;
QThread* TCPModbusThread;



void AlarmReset();

bool ReqState = false;
bool CurState = false;

void AlarmMassege1();
void AlarmMassege2();

QSound* play;
QString Path = "./sound.wav";


QMessageBox* massege;

signals:
void pixmapChanged();
void update_bd();


void startCommunication(const QString& address = "192.168.3.18", const int& port = 502);
void writeValueSignal(const int &ReqState);

public slots:

void On();
void Off();

private slots:

void AlarmCheck();
void AlarmSet();


protected:
void paintEvent(QPaintEvent *event);
};
#endif // QIMAGEWIDGET_H

inline int QImageWidget::counter {17};
