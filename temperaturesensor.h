#ifndef TSENSOR_H
#define TSENSOR_H

#include <QString>
#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <math.h>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>


class TSensor : public QWidget
{
Q_OBJECT

public:
    explicit TSensor (QString Location, QString parameter, QString unitGenitive);

    int values;

    bool temp1 = false;

    void setTimer(int timerMSec);

     float GetValues ();


signals:
     void update_bd();

     void ShowAlarmLowerLimitInSetting (int );
     void ShowAlarmUpperLimitInSetting (int );
     void ShowWarningUpperLimitInSetting (int );

public slots:

    void tempTemperatureChanger();

    void CheckAlarmLowerLimit();
    void CheckWarningLowerLimit();
    void CheckAlarmUpperLimit();
    void CheckWarningUpperLimit();

    void SetAlarmLowerLimit (int Limit);
    void SetWarningLowerLimit (int Limit);
    void SetAlarmUpperLimit (int Limit);
    void SetWarningUpperLimit (int Limit);

    void CancelSetAlarmLowerLimit ();
    void CancelSetAlarmUpperLimit ();
    void CancelSetWarningUpperLimit ();

private slots:
    void writeValueToSQL();


private:

  QString upperOrLowerTemperatureLimit;
  void AlarmMassege1(QString upperOrLowerTemperatureLimit);
  QMessageBox* massege;


    float ReadyValues;

    QString location;
    QString parameter;
    QString unit;
    QString fullNameLog;
    QString fullName;

    float AlarmLowerLimit;
    float WarningLowerLimit;
    float AlarmUpperLimit;
    float WarningUpperLimit;

    QTimer *timer;
    int timerMSec;

    bool emergyLowRange = false;
    bool warningLowRange = false;
    bool emergyUpperRange = false;
    bool warningUpperRange = false;

    QSqlQuery query;

};


#endif // TSENSOR_H
