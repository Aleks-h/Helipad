#ifndef ISUBSYSTEMWIDGET_H
#define ISUBSYSTEMWIDGET_H

#include <QTimer>
#include <QWidget>

class ISubsystemWidget : public QWidget
{
Q_OBJECT
public:

    ISubsystemWidget();
    virtual ~ISubsystemWidget();


protected:

    bool enableBit;
    bool ReqState;
    int cur_picture;

    int numberOfsubsystem;

    virtual void state(const int) = 0;


    bool button_ON_pushed;
    bool button_OFF_pushed;

    QTimer* timer2;
    QTimer* timer3;

private:

    void button_on_pushed();
    void button_off_pushed();

    void connect_timer2();
    void connect_timer3();





public slots:

    void On();
    void Off();

signals:

    bool writeValueSignal(int numberOfsubsystem, bool ReqState);

};

#endif // ISUBSYSTEMWIDGET_H
