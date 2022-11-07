#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMainWindow>
#include <QDateTime>
#include <QDebug>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QDialogButtonBox>
#include <QSettings>

#include <QLayout>
#include <QFormLayout>
#include <QSpinBox>
#include <QVector>

#include <QSlider>

#include "screeenshotwindow.h"

namespace Ui {
class Settings;
}

extern int sensorsCount;
extern QString locationName1;
extern QString locationName2;
extern QString locationName3;

class Settings : public QMainWindow
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();
    void setHashUserPassword (QByteArray HashValue);

    int currentLowerLimitAlarmChange1;
    int currentLowerLimitAlarmChange2;
    int currentLowerLimitAlarmChange3;

    int currentUpperLimitAlarmChange1;
    int currentUpperLimitAlarmChange2;
    int currentUpperLimitAlarmChange3;

    int currentUpperLimitWarningChange1;
    int currentUpperLimitWarningChange2;
    int currentUpperLimitWarningChange3;
//
  QVector<int> currentLowerLimitAlarmChange;
  QVector<int> currentUpperLimitAlarmChange;
  QVector<int> currentUpperLimitWarningChange;

public slots:
    //void ShowAlarmLowerLimit (int);

    void LowerLimitAlarmBufferSlot1 (int);
    void LowerLimitAlarmBufferSlot2 (int);
    void LowerLimitAlarmBufferSlot3 (int);

    void UpperLimitAlarmBufferSlot1 (int);
    void UpperLimitAlarmBufferSlot2 (int);
    void UpperLimitAlarmBufferSlot3 (int);

    void UpperLimitWarningBufferSlot1 (int);
    void UpperLimitWarningBufferSlot2 (int);
    void UpperLimitWarningBufferSlot3 (int);

    void ShowLowerLimitAlarm1 (int);
    void ShowLowerLimitAlarm2 (int);
    void ShowLowerLimitAlarm3 (int);

    void ShowUpperLimitAlarm1 (int);
    void ShowUpperLimitAlarm2 (int);
    void ShowUpperLimitAlarm3 (int);

    void ShowUpperLimitWarning1 (int);
    void ShowUpperLimitWarning2 (int);
    void ShowUpperLimitWarning3 (int);

public slots:
    void TakeAPictureSlot();
    void on_checkBox_toggled(bool checked);

private slots:
    void on_pushButton_13_clicked();
    void on_pushButton_14_clicked();
    void timerUpdate1();
    //void on_spinBox_valueChanged(const int &arg1);
    void on_pushButton_16_clicked();
    void on_pushButton_17_clicked();
    void on_pushButton_21_clicked();
    void on_pushButton_19_clicked();
    void on_pushButton_18_clicked();
    void on_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime);
    void on_ChangeSetting_clicked();
    void on_Exit_clicked();
    void on_ChangePassword_clicked();
    void on_TakeAScreenWindow_clicked();



    void on_TakeAPicture_clicked();

private:
    Ui::Settings *ui;
    screeenShotWindow *screen;

    void WidgetLayout(double widthCof, double heightUpCof, double heightButtomCof);

    int LowerLimitAlarmBuffer1;

    int BufAlarmLowerLimit;
    int CurAlarmLowerLimit;

    bool stopUpdateTime = false;
    QString dateTimeNoFormated;
    QString dateTimeFormated;
    QString format = "yyyyMMdd hh:mm:ss";

    QSqlQuery query;

    void logTimeIsChanged();
    void AlarmLowerLimitChanged(int sensorNumber);
    void AlarmUpperLimitChanged(int sensorNumber);
    void WarningUpperLimitChanged(int sensorNumber);

    void menuIsActive();
    void menuIsInactive();

    bool passwordEnter();

    QByteArray userPassword;
    QByteArray adminPassword = "7efc24e0d53bf7ae1bce33c58f1e9f076492250de28c6138fedaa41a7bfa45af";

    void saveSettingsToFile();



    void settingSensor (QFormLayout* temperetureSensorLayout, QLabel* SensorLocationObjct, QString locationName,
                        QLabel* LabelLowerLimitAlarmSensor, QSpinBox* LowerLimitAlarmChangeSpin,
                        QLabel* LabelUpperLimitAlarmSensor, QSpinBox* UpperLimitAlarmChangeSpin,
                        QLabel* LabelUpperLimitWarningSensor, QSpinBox* UpperLimitWarningChangeSpin,
                        int number);

     QFormLayout* temperetureSensor1Layout;
     QFormLayout* temperetureSensor2Layout;
     QFormLayout* temperetureSensor3Layout;


     QLabel* SensorLocationObjct1;
     QLabel* SensorLocationObjct2;
     QLabel* SensorLocationObjct3;


     QLabel* LabelLowerLimitAlarmSensor1;
     QLabel* LabelUpperLimitAlarmSensor1;
     QLabel* LabelUpperLimitWarningSensor1;

     QLabel* LabelLowerLimitAlarmSensor2;
     QLabel* LabelUpperLimitAlarmSensor2;
     QLabel* LabelUpperLimitWarningSensor2;

     QLabel* LabelLowerLimitAlarmSensor3;
     QLabel* LabelUpperLimitAlarmSensor3;
     QLabel* LabelUpperLimitWarningSensor3;

     QSpinBox* LowerLimitAlarmChangeSpin1;
     QSpinBox* UpperLimitAlarmChangeSpin1;
     QSpinBox* UpperLimitWarningChangeSpin1;

     QSpinBox* LowerLimitAlarmChangeSpin2;
     QSpinBox* UpperLimitAlarmChangeSpin2;
     QSpinBox* UpperLimitWarningChangeSpin2;

     QSpinBox* LowerLimitAlarmChangeSpin3;
     QSpinBox* UpperLimitAlarmChangeSpin3;
     QSpinBox* UpperLimitWarningChangeSpin3;

     QVector<QLabel*> SensorLocationObjct;
     QVector<QString> locationName;
     QVector<QFormLayout*> temperetureSensorLayout;
     QVector<QLabel*> LabelLowerLimitAlarmSensor;
     QVector<QLabel*>  LabelUpperLimitAlarmSensor;
     QVector<QLabel*> LabelUpperLimitWarningSensor;

     QVector<QSpinBox*> LowerLimitAlarmChangeSpin;

     QVector <QSpinBox*> UpperLimitAlarmChangeSpin;

     QVector <QSpinBox*> UpperLimitWarningChangeSpin;

signals:
     void firstWindow();
     void secondWindow();

     void SetAlarmLowerLimit1 (int);
     void SetAlarmLowerLimit2 (int);
     void SetAlarmLowerLimit3 (int);

     void SetAlarmUpperLimit1 (int);
     void SetAlarmUpperLimit2 (int);
     void SetAlarmUpperLimit3 (int);

     void SetWarningUpperLimit1 (int);
     void SetWarningUpperLimit2 (int);
     void SetWarningUpperLimit3 (int);

     void ShowAlarmLowerLimit1 (int);
     void ShowAlarmLowerLimit2 (int);
     void ShowAlarmLowerLimit3 (int);

     void ShowAlarmUpperLimit1 (int);
     void ShowAlarmUpperLimit2 (int);
     void ShowAlarmUpperLimit3 (int);

     void ShowWarningUpperLimit1 (int);
     void ShowWarningUpperLimit2 (int);
     void ShowWarningUpperLimit3 (int);

     void CancelSetAlarmLimit();


     void update_bd();
     void AlarmChangeSpinSetEnable(bool);
     void TakeAPictureSignal();

     void TakeAPictureButtonVisibilitySignal(bool&);

};



#endif // SETTINGS_H
