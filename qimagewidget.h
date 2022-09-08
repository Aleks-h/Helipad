#ifndef QIMAGEWIDGET_H
#define QIMAGEWIDGET_H
#include <QWidget>
#include <QPixmap>
#include "QPainter"
#include <QMessageBox>
#include <QSound>

class QImageWidget : public QWidget
{
Q_OBJECT
public:
explicit QImageWidget (QString);
QString name;
void state(const int);
void setPixmap(QPixmap pixmap);


private:
QPixmap _originalImage;
int cur_picture_prev;
void updateMargins();
int cur_picture;
bool ISalarm = 0;



void AlarmSet();
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

public slots:
void alarm();
void On();
void Off();



protected:
void paintEvent(QPaintEvent *event);
};
#endif // QIMAGEWIDGET_H
