#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "ui_mainwindow.h"
#include <QMainWindow>
#include "database.h"
#include <QPushButton>
#include "history_screen.h"
#include "settings.h"
#include "temperaturesensor.h"
#include <QDateTime>
#include "./qimagewidget.h"
#include <QLabel>
#include <QVector>
#include <QTimer>
#include <QDebug>
#include <QSqlTableModel>
#include <QSettings>
#include <QTextCodec>
#include <QGridLayout>
#include <QTextBrowser>


#include <QLayout>
#include <QRect>
#include <QWidgetItem>
#include <QFormLayout>
#include <QLCDNumber>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// extern int sensorsCount;
// extern QString locationName1;
// extern QString locationName2;
// extern QString locationName3;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Database   *db;

  //static void VisibilityOfTakeAScreenButton (bool IsButtonVisibility);
  //static bool screenButtonState;


public slots:
    void on_pushButton_7_clicked();

    void timerUpdate();



    void sensorShow(TSensor &Sensor, QLCDNumber &Sensorvalue);
    void sensorCheckLowerLimit();

    void on_pushButton_9_clicked();

     void TakeAPicture();

  //  void ShowAlarmLowerLimitInSetting1 (int);

signals:
    void alarm1();
    void alarm2();
    void alarm3();

    void SetAlarmLowerLimit1FromConfFile(int);
    void SetAlarmLowerLimit2FromConfFile(int);
    void SetAlarmLowerLimit3FromConfFile(int);

    void SetAlarmUpperLimit1FromConfFile(int);
    void SetAlarmUpperLimit2FromConfFile(int);
    void SetAlarmUpperLimit3FromConfFile(int);

    void SetWarningUpperLimit1FromConfFile(int);
    void SetWarningUpperLimit2FromConfFile(int);
    void SetWarningUpperLimit3FromConfFile(int);

    void TakeAPicktureSignal();


private slots:
    void on_TakeAScreen_clicked();

private:

    Ui::MainWindow *ui;
    History_screen *window;
    Settings *settings;

    bool button_state1 = false;
    bool button_state2 = false;
    bool button_state3 = false;



    QString str;
    QDateTime time;
    QTimer *timer;
    QTimer *timer1;

    void sensors (TSensor* sensor1, TSensor* sensor2, TSensor* sensor3, QLabel* locationNameLabel, QString* locationName, QVBoxLayout* TemperLayout,
                    QVBoxLayout* TemperLayoutFull, QFormLayout* FormLayout,  QLabel* sensorNameLabel1, QString* sensorName1,
                   QLCDNumber* sensorValue1,   QLabel* sensorNameLabel2, QString* sensorName2,
                  QLCDNumber* sensorValue2,  QLabel* sensorNameLabel3, QString* sensorName3,
                  QLCDNumber* sensorValue3, QFrame* Frame, QSpacerItem* VSpItemTemp, QString* unit1, QString* unit2, QString* unit3, int numberSensor);


    QSpacerItem* HSpItem;

    QList<TSensor*> sensor1;
    QList<TSensor*> sensor2;
    QList<TSensor*> sensor3;

    QList<QLabel*> locationNameLabel;
    QList<QString*> locationName;
    QList<QVBoxLayout*> TemperLayout;
    QList<QVBoxLayout*> TemperLayoutFull;
    QList<QFormLayout*> FormLayout;

    QList<QLabel*> sensorNameLabel1;
    QList<QString*> sensorName1;
    QList<QLCDNumber*> sensorValue1;

    QList<QLabel*> sensorNameLabel2;
    QList<QString*> sensorName2;
    QList<QLCDNumber*> sensorValue2;

    QList<QLabel*> sensorNameLabel3;
    QList<QString*> sensorName3;
    QList<QLCDNumber*> sensorValue3;

    QList<QFrame*> Frame;
    QList<QSpacerItem*> VSpItemTemp;
    QList<QString*> unit1;
    QList<QString*> unit2;
    QList<QString*> unit3;

    class TSensor* sensor1_1;
    class TSensor* sensor1_2;
    class TSensor* sensor1_3;
    QLabel* locationNameLabel1;

    QVBoxLayout* TemperLayout1;
    QVBoxLayout* TemperLayoutFull1;
    QFormLayout* FormLayout1;

    QLabel* sensorNameLabel1_1;
    QLabel* sensorNameLabel1_2;
    QLabel* sensorNameLabel1_3;

    QString sensorName1_1;
    QString sensorName1_2;
    QString sensorName1_3;

    QLCDNumber* sensorValue1_1;
    QLCDNumber* sensorValue1_2;
    QLCDNumber* sensorValue1_3;

    QFrame* Frame1;
    QSpacerItem* VSpItemTemp1;
    QString unit1_1;
    QString unit1_2;
    QString unit1_3;


    class TSensor* sensor2_1;
    class TSensor* sensor2_2;
    class TSensor* sensor2_3;
    QLabel* locationNameLabel2;

    QVBoxLayout* TemperLayout2;
    QVBoxLayout* TemperLayoutFull2;
    QFormLayout* FormLayout2;

    QLabel* sensorNameLabel2_1;
    QLabel* sensorNameLabel2_2;
    QLabel* sensorNameLabel2_3;

    QString sensorName2_1;
    QString sensorName2_2;
    QString sensorName2_3;

    QLCDNumber* sensorValue2_1;
    QLCDNumber* sensorValue2_2;
    QLCDNumber* sensorValue2_3;

    QFrame* Frame2;
    QSpacerItem* VSpItemTemp2;
    QString unit2_1;
    QString unit2_2;
    QString unit2_3;


    class TSensor* sensor3_1;
    class TSensor* sensor3_2;
    class TSensor* sensor3_3;
    QLabel* locationNameLabel3;

    QVBoxLayout* TemperLayout3;
    QVBoxLayout* TemperLayoutFull3;
    QFormLayout* FormLayout3;

    QLabel* sensorNameLabel3_1;
    QLabel* sensorNameLabel3_2;
    QLabel* sensorNameLabel3_3;

    QString sensorName3_1;
    QString sensorName3_2;
    QString sensorName3_3;

    QLCDNumber* sensorValue3_1;
    QLCDNumber* sensorValue3_2;
    QLCDNumber* sensorValue3_3;

    QFrame* Frame3;
    QSpacerItem* VSpItemTemp3;
    QString unit3_1;
    QString unit3_2;
    QString unit3_3;

    void readConfigFromFile();
    void readSettingsFromFile();

    int numberOfSubsystem;

    void initVariablesSubsys();
    void initVariablesSensors();



    QList<QVBoxLayout*> VLay;
    QList<QVBoxLayout*> VLayInd;
    QList<QSpacerItem*> VSpItem;
    QList<QLabel*> Label;
    QList<QPushButton*> PButOn;
    QList<QImageWidget*> Indic;
    QList<QPushButton*> PButOff;
    QList<QString*> name;

    void subsystem (QVBoxLayout* VLay, QVBoxLayout* VLayInd, QSpacerItem* VSpItem, QLabel* Label,
                    QPushButton* PButOn, QImageWidget* Indic, QPushButton* PButOff, QString* name);


    QVBoxLayout* VLay1;
    QVBoxLayout* VLayInd1;
    QSpacerItem* VSpItem1;
    QLabel* Label1;
    QPushButton* PButOn1;
    QImageWidget* Indic1;
    QPushButton* PButOff1;
    QString name1;

    QVBoxLayout* VLay2;
    QVBoxLayout* VLayInd2;
    QSpacerItem* VSpItem2;
    QLabel* Label2;
    QPushButton* PButOn2;
    QImageWidget* Indic2;
    QPushButton* PButOff2;
    QString name2;

    QVBoxLayout* VLay3;
    QVBoxLayout* VLayInd3;
    QSpacerItem* VSpItem3;
    QLabel* Label3;
    QPushButton* PButOn3;
    QImageWidget* Indic3;
    QPushButton* PButOff3;
    QString name3;

    QVBoxLayout* VLay4;
    QVBoxLayout* VLayInd4;
    QSpacerItem* VSpItem4;
    QLabel* Label4;
    QPushButton* PButOn4;
    QImageWidget* Indic4;
    QPushButton* PButOff4;
    QString name4;

    QVBoxLayout* VLay5;
    QVBoxLayout* VLayInd5;
    QSpacerItem* VSpItem5;
    QLabel* Label5;
    QPushButton* PButOn5;
    QImageWidget* Indic5;
    QPushButton* PButOff5;
    QString name5;

    QVBoxLayout* VLay6;
    QVBoxLayout* VLayInd6;
    QSpacerItem* VSpItem6;
    QLabel* Label6;
    QPushButton* PButOn6;
    QImageWidget* Indic6;
    QPushButton* PButOff6;
    QString name6;

    QVBoxLayout* VLay7;
    QVBoxLayout* VLayInd7;
    QSpacerItem* VSpItem7;
    QLabel* Label7;
    QPushButton* PButOn7;
    QImageWidget* Indic7;
    QPushButton* PButOff7;
    QString name7;

    QVBoxLayout* VLay8;
    QVBoxLayout* VLayInd8;
    QSpacerItem* VSpItem8;
    QLabel* Label8;
    QPushButton* PButOn8;
    QImageWidget* Indic8;
    QPushButton* PButOff8;
    QString name8;

    QVBoxLayout* VLay9;
    QVBoxLayout* VLayInd9;
    QSpacerItem* VSpItem9;
    QLabel* Label9;
    QPushButton* PButOn9;
    QImageWidget* Indic9;
    QPushButton* PButOff9;
    QString name9;

    QVBoxLayout* VLay10;
    QVBoxLayout* VLayInd10;
    QSpacerItem* VSpItem10;
    QLabel* Label10;
    QPushButton* PButOn10;
    QImageWidget* Indic10;
    QPushButton* PButOff10;
    QString name10;

};


#endif // MAINWINDOW_H
