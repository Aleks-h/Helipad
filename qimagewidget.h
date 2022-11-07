#ifndef QIMAGEWIDGET_H
#define QIMAGEWIDGET_H

#include <QPixmap>
#include "QPainter"
#include <QMessageBox>
#include <QSound>
#include <QThread>

#include <QTimer>

#include "isubsystemwidget.h"

class QImageWidget : public ISubsystemWidget
{
Q_OBJECT
public:
QImageWidget (QString& name, int& numberOfSub, QVector<bool>& CurState);
virtual~QImageWidget();
QString name;
void setPixmap(QPixmap pixmap);


private:
QPixmap _originalImage;

QTimer* timer;
QTimer* timer1;

int cur_picture_prev;
void updateMargins();

bool ISalarm = 0;

void state(const int) override;

void AlarmReset();




bool& CurState;


void AlarmMassege1();
void AlarmMassege2();

QSound* play;
QString Path = "./sound.wav";


QMessageBox* massege;

signals:

void pixmapChanged();
void update_bd();

public slots:

void On();
void Off();

void setEnabled(bool);

private slots:

void AlarmCheck();
void AlarmSet();


protected:
void paintEvent(QPaintEvent *event);
};
#endif // QIMAGEWIDGET_H

