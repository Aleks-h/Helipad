#include "isubsystemwidget.h"

ISubsystemWidget::ISubsystemWidget():
                                     enableBit{false}
                                    ,ReqState{0}
                                    ,cur_picture{0}
                                    ,button_ON_pushed{false}
                                    ,button_OFF_pushed{false}
                                    ,timer2{nullptr}
                                    ,timer3{nullptr}
{

}

ISubsystemWidget::~ISubsystemWidget()
{
delete timer2;
delete timer3;
}


void ISubsystemWidget::button_on_pushed()
{
    button_ON_pushed = true;
    if(timer2 == nullptr)
    {
    timer2 = new QTimer();
    }
    timer2->setSingleShot(true);
 //   connect_timer2();

    timer2->start(1700);
}

void ISubsystemWidget::button_off_pushed()
{
    button_OFF_pushed = true;
    if(timer3 == nullptr)
    {
    timer3 = new QTimer();
    }
    timer3->setSingleShot(true);
  //  connect_timer3();
    timer3->start(1700);
}


void ISubsystemWidget::On()
{
if(enableBit)
    return;

button_on_pushed();

//if(cur_picture == 1)
 //  return;

if(ReqState == true)
    return;

cur_picture = 1;
ReqState = true;

//emit writeValueSignal(numberOfsubsystem, ReqState);
state(cur_picture);
}

void ISubsystemWidget::Off()
{
if(enableBit)
    return;

button_off_pushed();

if(cur_picture == 0)
    return;

//if(cur_picture == 0)
 //   return;

cur_picture = 0;
ReqState = false;
//emit writeValueSignal(numberOfsubsystem, ReqState);
state(cur_picture);
}
