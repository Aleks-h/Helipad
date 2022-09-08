#include "temperaturesensor.h"
#include "QDebug"


TSensor::TSensor(QString location, QString parameter, QString unitGenitive)
{
    timer = new QTimer (this);
    connect(timer,SIGNAL(timeout()),this,SLOT(writeValueToSQL()));
    timer->start(timerMSec);
    this->location = location;
    this->parameter = parameter;
    this->unit = unitGenitive;
    fullNameLog = "Логирование_"+parameter+"_"+location;
    fullName = parameter +"("+location+")";
    timerMSec = 180000000;
    values = 10000;
}

void TSensor::setTimer(int timerMSec)
{
this->timerMSec = timerMSec;
}

float TSensor::GetValues()
{
    ReadyValues = ((float)values)/100;
    return ReadyValues;
}

void TSensor::SetAlarmLowerLimit(int Limit)
{
AlarmLowerLimit = Limit;
emit ShowAlarmLowerLimitInSetting (Limit);
}

void TSensor::SetWarningLowerLimit(int Limit)
{
WarningLowerLimit = Limit;
}

void TSensor::SetAlarmUpperLimit (int Limit)
{
AlarmUpperLimit = Limit;
emit ShowAlarmUpperLimitInSetting (Limit);
}

void TSensor::SetWarningUpperLimit (int Limit)
{
WarningUpperLimit = Limit;
emit ShowWarningUpperLimitInSetting (Limit);
}

void TSensor::CancelSetAlarmLowerLimit()
{
emit ShowAlarmLowerLimitInSetting (AlarmLowerLimit);
}

void TSensor::CancelSetAlarmUpperLimit ()
{
emit ShowAlarmUpperLimitInSetting (AlarmUpperLimit);
}

void TSensor::CancelSetWarningUpperLimit ()
{
emit ShowWarningUpperLimitInSetting (WarningUpperLimit);
}

void TSensor::CheckAlarmLowerLimit()
{
    QString StringAlarmLowerLimit;
    if (ReadyValues < AlarmLowerLimit)
    {
        if (!emergyLowRange)
        {
        upperOrLowerTemperatureLimit = "нижний";
        StringAlarmLowerLimit = QString::number(AlarmLowerLimit);
        query.prepare("insert into log (Event, date, Time) values (:values, CURRENT_DATE(), CURRENT_TIME())");
        query.bindValue(":values", "Авария: " + fullName + " ниже " + StringAlarmLowerLimit + ' ' + unit);
        query.exec();
        emergyLowRange  = true;
        AlarmMassege1(upperOrLowerTemperatureLimit);
        emit update_bd();
        }
    }
    if (ReadyValues > (AlarmLowerLimit + 1))
    {
        if (emergyLowRange)
        {

        QSqlQuery query;
        StringAlarmLowerLimit = QString::number(AlarmLowerLimit + 1);
        query.prepare("insert into log (Event, date, Time) values (:values, CURRENT_DATE(), CURRENT_TIME())");
        query.bindValue(":values", fullName + " норма (выше " + StringAlarmLowerLimit + ' ' + unit + ')');
        query.exec();
        emergyLowRange = false;
        emit update_bd();
        }

    }

}



void TSensor::CheckWarningLowerLimit()
{
    ReadyValues < WarningLowerLimit;
}

void TSensor::CheckAlarmUpperLimit()
{
    ReadyValues > AlarmUpperLimit;
}

void TSensor::CheckWarningUpperLimit()
{
    ReadyValues > WarningUpperLimit;
}

void TSensor::AlarmMassege1(QString upperOrLowerTemperatureLimit)
{
    massege = new QMessageBox;
    massege -> setText(fullName + ": авария предела температуры");
    massege -> setDetailedText("Достигнут " + upperOrLowerTemperatureLimit + " предел температуры");
    massege -> addButton(tr("Подтвердить"), QMessageBox::ActionRole);
    massege -> setIcon(QMessageBox::Critical);
    massege -> exec();
    delete massege;
    if (massege->clickedButton()) {
        query.prepare("insert into log (Event, date, Time) values (:name, CURRENT_DATE(), CURRENT_TIME());");
        query.bindValue(":name", fullName + ". Авария подтверждена.");
        query.exec();
    }
}



void TSensor::tempTemperatureChanger()
{
if (values < 450)
{
    temp1 = true;
}

if (values > 550)
{
   temp1 = false;
}

if (temp1)
{values = values + 10;}
else {values = values - 10;}
}



void TSensor::writeValueToSQL()
{
    query.prepare(QString("insert into %1 (Event, date, Time) values (:values, CURRENT_DATE(), CURRENT_TIME())").arg(fullNameLog));
    query.bindValue(":values", (round(ReadyValues*10))/10);
    query.exec();
}

