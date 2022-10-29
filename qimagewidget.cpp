#include "qimagewidget.h"
#include <QDebug>
#include <database.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QSound>


QImageWidget::QImageWidget(QString& name, int& numberOfSub,
                                   QVector<bool>& CurState)
                                      : cur_picture{0},
                                        button_ON_pushed{false},
                                        button_OFF_pushed{false},
                                        numberOfsubsystem{numberOfSub},
                                        ReqState{0},
                                        CurState{CurState[numberOfSub]}


{
connect (this, SIGNAL(pixmapChanged()), SLOT(repaint()));
this -> name = name;
cur_picture_prev = 0;

timer = new QTimer(this);
timer1 = new QTimer(this);
connect(timer,&QTimer::timeout, this, &QImageWidget::AlarmCheck);
timer->start(1000);


}

QImageWidget::~QImageWidget()
{
delete timer;
delete timer1;
delete timer2;
delete timer3;
}


void QImageWidget::setPixmap(QPixmap pixmap)
{
_originalImage = pixmap;
emit pixmapChanged();
}

void QImageWidget::button_on_pushed()
{
    button_ON_pushed = true;
    if(timer2 == nullptr)
    {
    timer2 = new QTimer(this);
    }
    timer2->setSingleShot(true);
    connect (timer2, &QTimer::timeout, [&](){
                                             button_ON_pushed = false;
                                            });
    timer2->start(1700);
}

void QImageWidget::button_off_pushed()
{
    button_OFF_pushed = true;
    if(timer3 == nullptr)
    {
    timer3 = new QTimer(this);
    }
    timer3->setSingleShot(true);
    connect (timer3, &QTimer::timeout, [&](){
                                             button_OFF_pushed = false;
                                            });
    timer3->start(1700);
}


void QImageWidget::On()
{
button_on_pushed();

//if(cur_picture == 1)
 //  return;

if(ReqState == true)
    return;

cur_picture = 1;
ReqState = true;

emit writeValueSignal(numberOfsubsystem, ReqState);
state(cur_picture);
}

void QImageWidget::Off()
{
button_off_pushed();

if(cur_picture == 0)
    return;

//if(cur_picture == 0)
 //   return;

cur_picture = 0;
ReqState = false;
emit writeValueSignal(numberOfsubsystem, ReqState);
state(cur_picture);
}

void QImageWidget::state(int cur_picture){

    if (cur_picture == 2 and cur_picture_prev != cur_picture)
    {
    cur_picture_prev = cur_picture;
    setPixmap(QPixmap("./Pictures/State_3.png"));
    QSqlQuery query;
    query.prepare("insert into log (Event, date, Time) values (:name, CURRENT_DATE(), CURRENT_TIME());");
    QSound* play = new QSound (Path);
      play -> setLoops(QSound::Infinite);
      play -> play();
      if (ReqState)
      {
       query.bindValue(":name", "Авария работы " + name);
       query.exec();
       AlarmMassege1();
      }
      else
      {
       query.bindValue(":name", "Авария " + name);
       query.exec();
       AlarmMassege2();
      }
    delete play;
   }

    if (cur_picture == 0 and cur_picture_prev != cur_picture)
    {
    if (!ISalarm) {
    cur_picture_prev = cur_picture;
    setPixmap(QPixmap("./Pictures/State_1.png"));
    QSqlQuery query;
    query.prepare("insert into log (Event, date, Time) values (:name, CURRENT_DATE(), CURRENT_TIME());");
    query.bindValue(":name", name + " отключен");
    query.exec();
    }
    }


    if (cur_picture == 1 and cur_picture_prev != cur_picture)
    {

    if (!ISalarm) {
    cur_picture_prev = cur_picture;
    setPixmap(QPixmap("./Pictures/State_2.png"));

    QSqlQuery query;
    query.prepare("insert into log (Event, date, Time) values (:name, CURRENT_DATE(), CURRENT_TIME());");
    query.bindValue(":name", name + " включен");
    query.exec();
    }
    }
     emit update_bd();
    }

void QImageWidget::AlarmMassege1()
{
    massege = new QMessageBox;
    massege -> setText("Авария работы подсистемы " + name);
    massege -> setDetailedText("В секцию управления не возвращаетя обратная связь с подсистемы о включении");
    massege -> addButton(tr("Подтвердить"), QMessageBox::ActionRole);
    massege -> setIcon(QMessageBox::Critical);
    massege -> exec();
    delete massege;
    if (massege->clickedButton()) {
        QSqlQuery query;
        query.prepare("insert into log (Event, date, Time) values (:name, CURRENT_DATE(), CURRENT_TIME());");
        query.bindValue(":name", "Авария работы подсистемы " + name + " подтверждена");
        query.exec();
    }
}

void QImageWidget::AlarmMassege2()
{
    massege = new QMessageBox;
    massege -> setText("Авария подсистемы " + name);
    massege -> setDetailedText("В секцию управления приходит обратная связь с отключенной подсистемы");
    massege -> addButton(tr("Подтвердить"), QMessageBox::ActionRole);
    massege -> setIcon(QMessageBox::Critical);
    massege -> exec();
    delete massege;
    if (massege->clickedButton()) {
        QSqlQuery query;
        query.prepare("insert into log (Event, date, Time) values (:name, CURRENT_DATE(), CURRENT_TIME());");
        query.bindValue(":name", "Авария подсистемы " + name + " подтверждена");
        query.exec();
    }
}


void QImageWidget::paintEvent(QPaintEvent *)
{
QPainter painter(this);

if (_originalImage.isNull()){
setPixmap(QPixmap("./Pictures/State_1.png"));
}
else {
QRect imageRect = rect();
painter.drawPixmap(imageRect, _originalImage);
}
}



void QImageWidget::AlarmCheck()
{
    if (CurState != ReqState and !(timer1->isActive()) and !ISalarm)
    {
     timer1->setSingleShot(true);
     connect(timer1,&QTimer::timeout, this, &QImageWidget::AlarmSet);
     timer1->start(700);
     }
    if (CurState == ReqState and ISalarm)
     {
     AlarmReset();
     }
}

void QImageWidget::AlarmSet()
{
    if (CurState != ReqState and (!button_ON_pushed or !button_OFF_pushed))
    {
     ISalarm = true;
     cur_picture = 2;
     state(cur_picture);
    }
}

void QImageWidget::AlarmReset()
{

        if (CurState)
        {
            cur_picture = 1;
        }
        else
        {
            cur_picture = 0;
        }
        ISalarm = false;
}




