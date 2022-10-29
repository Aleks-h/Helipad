#ifndef QIMAGEWIDGET_H
#define QIMAGEWIDGET_H
#include <QWidget>
#include <QPixmap>
#include "QPainter"
#include <QMessageBox>
#include <QSound>
#include <QThread>

#include <QTimer>

class QImageWidget : public QWidget
{
Q_OBJECT
public:
QImageWidget (QString& name, int& numberOfSub, QVector<bool>& CurState);
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



void AlarmReset();

int numberOfsubsystem;

bool ReqState;
bool& CurState;


void AlarmMassege1();
void AlarmMassege2();

QSound* play;
QString Path = "./sound.wav";


QMessageBox* massege;

signals:
void pixmapChanged();
void update_bd();

bool writeValueSignal(int numberOfsubsystem, bool ReqState);


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

