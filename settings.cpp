#include "settings.h"
#include "ui_settings.h"

#include "QDebug"

Settings::Settings(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::Settings)
{

    currentLowerLimitAlarmChange = {currentLowerLimitAlarmChange1,
                                    currentLowerLimitAlarmChange2,
                                    currentLowerLimitAlarmChange3
                                    };

    currentUpperLimitAlarmChange = {currentUpperLimitAlarmChange1,
                                    currentUpperLimitAlarmChange2,
                                    currentUpperLimitAlarmChange3
                                    };

    currentUpperLimitWarningChange = {currentUpperLimitWarningChange1,
                                    currentUpperLimitWarningChange2,
                                    currentUpperLimitWarningChange3
                                    };

    LabelLowerLimitAlarmSensor = {LabelLowerLimitAlarmSensor1,
                                 LabelLowerLimitAlarmSensor2,
                                 LabelLowerLimitAlarmSensor3};

    LabelUpperLimitAlarmSensor = {LabelUpperLimitAlarmSensor1,
                                 LabelUpperLimitAlarmSensor2,
                                 LabelUpperLimitAlarmSensor3};

    LabelUpperLimitWarningSensor = {LabelUpperLimitWarningSensor1,
                                   LabelUpperLimitWarningSensor2,
                                   LabelUpperLimitWarningSensor3};

    locationName = {locationName1,locationName2,locationName3};

    SensorLocationObjct = {SensorLocationObjct1,
                           SensorLocationObjct2,
                           SensorLocationObjct3};

    temperetureSensorLayout = {temperetureSensor1Layout,
                               temperetureSensor2Layout,
                               temperetureSensor3Layout};

    LowerLimitAlarmChangeSpin = {LowerLimitAlarmChangeSpin1,
                                 LowerLimitAlarmChangeSpin2,
                                 LowerLimitAlarmChangeSpin3};

    UpperLimitAlarmChangeSpin = {UpperLimitAlarmChangeSpin1,
                                 UpperLimitAlarmChangeSpin2,
                                 UpperLimitAlarmChangeSpin3};


    UpperLimitWarningChangeSpin = {UpperLimitWarningChangeSpin1,
                                  UpperLimitWarningChangeSpin2,
                                  UpperLimitWarningChangeSpin3};

    ui -> setupUi(this);
    ui -> pushButton_18 -> setEnabled(false);
    ui -> pushButton_19 -> setEnabled(false);
    ui -> dateTimeEdit -> setEnabled(false);
    ui -> pushButton_21 -> setEnabled(true);
    ui -> label_3 -> setEnabled(false);
    ui -> checkBox -> setEnabled(false);
    ui -> TakeAScreenWindow -> setEnabled(false);
    ui->statusbar->showMessage("Москва, ООО 'Аеросвет', 2022");

    ui->TakeAPicture->hide();

    menuIsInactive();

    for (int i=0; i < sensorsCount; ++i)
    {
    settingSensor (temperetureSensorLayout.at(i), SensorLocationObjct.at(i), locationName.at(i),
                  LabelLowerLimitAlarmSensor.at(i), LowerLimitAlarmChangeSpin.at(i),
                  LabelUpperLimitWarningSensor.at(i), UpperLimitAlarmChangeSpin.at(i),
                  LabelUpperLimitWarningSensor.at(i), UpperLimitWarningChangeSpin.at(i),
                  i);

    }

    screen = new screeenShotWindow (this);
    connect(screen, &screeenShotWindow::exit, this, &Settings::showFullScreen);
    connect(this, &Settings::TakeAPictureSignal, screen, &screeenShotWindow::TakeAPicture);

    connect(this, &Settings::TakeAPictureButtonVisibilitySignal, screen, &screeenShotWindow::TakeAPictureButtonVisibilitySlot);

}

Settings::~Settings()
{
    delete ui;
}

void Settings::setHashUserPassword(QByteArray HashValue)
{
    this->userPassword = HashValue;
}


void Settings::on_pushButton_13_clicked()
{
    menuIsInactive();
    this->close();
    emit firstWindow();
}

void Settings::on_pushButton_14_clicked()
{
    menuIsInactive();
    this->close();
    emit secondWindow();
}


void Settings::timerUpdate1()
{
    if (!stopUpdateTime)
    {
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    }
}

void Settings::on_pushButton_16_clicked()
{
    emit SetAlarmLowerLimit1(currentLowerLimitAlarmChange.at(0));
    emit SetAlarmLowerLimit2(currentLowerLimitAlarmChange.at(1));
    emit SetAlarmLowerLimit3(currentLowerLimitAlarmChange.at(2));

    emit SetAlarmUpperLimit1(currentUpperLimitAlarmChange.at(0));
    emit SetAlarmUpperLimit2(currentUpperLimitAlarmChange.at(1));
    emit SetAlarmUpperLimit3(currentUpperLimitAlarmChange.at(2));

    emit SetWarningUpperLimit1(currentUpperLimitWarningChange.at(0));
    emit SetWarningUpperLimit2(currentUpperLimitWarningChange.at(1));
    emit SetWarningUpperLimit3(currentUpperLimitWarningChange.at(2));

query.prepare("insert into log (Event, date, Time) values (:values, CURRENT_DATE(), CURRENT_TIME())");
query.bindValue(":values","Значения уставок температуры изменены:");
query.exec();
    for(int i = 0; i<sensorsCount; i++)
    {
    AlarmLowerLimitChanged(i);
    AlarmUpperLimitChanged(i);
    WarningUpperLimitChanged(i);
    }
saveSettingsToFile();
emit update_bd();
}

void Settings::on_TakeAScreenWindow_clicked()
{
        this -> close ();
        screen -> showFullScreen();
}


void Settings::AlarmLowerLimitChanged(int sensorNumber)
{
    QString s = QString::number(currentLowerLimitAlarmChange.at(sensorNumber));

    query.prepare("insert into log (Event) values (:values)");
    query.bindValue(":values",locationName.at(sensorNumber)+". Мин. аварийное значение: "
                    + s + " гр.");
    query.exec();
}

void Settings::AlarmUpperLimitChanged(int sensorNumber)
{
    QString s = QString::number(currentUpperLimitAlarmChange.at(sensorNumber));

    query.prepare("insert into log (Event) values (:values)");
    query.bindValue(":values",locationName.at(sensorNumber)+". Макс. аварийное значение: "
                    + s + " гр.");
    query.exec();
}

void Settings::WarningUpperLimitChanged(int sensorNumber)
{
    QString s = QString::number(currentUpperLimitWarningChange.at(sensorNumber));

    query.prepare("insert into log (Event) values (:values)");
    query.bindValue(":values",locationName.at(sensorNumber)+". Макс. значение-предупреждение: "
                    + s + " гр.");
    query.exec();
}

void Settings::saveSettingsToFile()
{
  QSettings settings("settings.ini",QSettings::IniFormat);
  settings.beginGroup("Settings");
  settings.setValue( "password", userPassword);
  settings.beginGroup("sensor1");
  settings.setValue( "AlarmLowTempLimit", currentLowerLimitAlarmChange.at(0));
  settings.setValue( "AlarmHighTempLimit", currentUpperLimitAlarmChange.at(0));
  settings.setValue( "WarningHighTempLimit", currentUpperLimitWarningChange.at(0));
  settings.endGroup();
  settings.beginGroup("sensor2");
  settings.setValue( "AlarmLowTempLimit", currentLowerLimitAlarmChange.at(1));
  settings.setValue( "AlarmHighTempLimit", currentUpperLimitAlarmChange.at(1));
  settings.setValue( "WarningHighTempLimit", currentUpperLimitWarningChange.at(1));
  settings.endGroup();
  settings.beginGroup("sensor3");
  settings.setValue("AlarmLowTempLimit", currentLowerLimitAlarmChange.at(2));
  settings.setValue( "AlarmHighTempLimit", currentUpperLimitAlarmChange.at(2));
  settings.setValue( "WarningHighTempLimit", currentUpperLimitWarningChange.at(2));
  settings.endGroup();
  settings.endGroup();
}

void Settings::LowerLimitAlarmBufferSlot1(int AlarmLowerLimit)
{
this->currentLowerLimitAlarmChange[0] = AlarmLowerLimit;
}

void Settings::LowerLimitAlarmBufferSlot2(int AlarmLowerLimit)
{
this->currentLowerLimitAlarmChange[1] = AlarmLowerLimit;
}

void Settings::LowerLimitAlarmBufferSlot3(int AlarmLowerLimit)
{
this->currentLowerLimitAlarmChange[2] = AlarmLowerLimit;
}



void Settings::UpperLimitAlarmBufferSlot1(int AlarmUpperLimit)
{
    this->currentUpperLimitAlarmChange[0] = AlarmUpperLimit;
}

void Settings::UpperLimitAlarmBufferSlot2(int AlarmUpperLimit)
{
    this->currentUpperLimitAlarmChange[1] = AlarmUpperLimit;
}

void Settings::UpperLimitAlarmBufferSlot3(int AlarmUpperLimit)
{
     this->currentUpperLimitAlarmChange[2] = AlarmUpperLimit;
}



void Settings::UpperLimitWarningBufferSlot1(int WarningUpperLimit)
{
     this->currentUpperLimitWarningChange[0] = WarningUpperLimit;
}

void Settings::UpperLimitWarningBufferSlot2(int  WarningUpperLimit)
{
    this->currentUpperLimitWarningChange[1] = WarningUpperLimit;
}

void Settings::UpperLimitWarningBufferSlot3(int WarningUpperLimit)
{
    this->currentUpperLimitWarningChange[2] = WarningUpperLimit;
}



void Settings::ShowLowerLimitAlarm1(int value)
{
 emit ShowAlarmLowerLimit1 (value);
}

void Settings::ShowLowerLimitAlarm2(int value)
{
 emit ShowAlarmLowerLimit2 (value);
}

void Settings::ShowLowerLimitAlarm3(int value)
{
 emit ShowAlarmLowerLimit3 (value);
}

void Settings::ShowUpperLimitAlarm1(int value)
{
 emit ShowAlarmUpperLimit1 (value);
}

void Settings::ShowUpperLimitAlarm2(int value)
{
 emit ShowAlarmUpperLimit2 (value);
}

void Settings::ShowUpperLimitAlarm3(int value)
{
 emit ShowAlarmUpperLimit3 (value);
}

void Settings::ShowUpperLimitWarning1(int value)
{
 emit ShowWarningUpperLimit1 (value);
}

void Settings::ShowUpperLimitWarning2(int value)
{
 emit ShowWarningUpperLimit2 (value);
}

void Settings::ShowUpperLimitWarning3(int value)
{
 emit ShowWarningUpperLimit3 (value);
}

void Settings::on_pushButton_17_clicked()
{
    emit CancelSetAlarmLimit();
}

void Settings::on_pushButton_21_clicked()
{
     this -> dateTimeFormated = QDateTime::currentDateTime().toString(format);
     ui -> pushButton_18 -> setEnabled(true);
     ui -> pushButton_19 -> setEnabled(true);
     ui -> dateTimeEdit -> setEnabled(true);
     ui -> pushButton_21 -> setEnabled(false);
     stopUpdateTime = true;
}

void Settings::on_pushButton_19_clicked()
{
    ui -> pushButton_18 -> setEnabled(false);
    ui -> pushButton_19 -> setEnabled(false);
    ui -> dateTimeEdit -> setEnabled(false);
    ui -> pushButton_21 -> setEnabled(true);
    stopUpdateTime = false;
}

void Settings::on_pushButton_18_clicked()
{
    ui -> pushButton_18 -> setEnabled(false);
    ui -> pushButton_19 -> setEnabled(false);
    ui -> dateTimeEdit -> setEnabled(false);
    ui -> pushButton_21 -> setEnabled(true);
    this -> dateTimeFormated = "sudo /usr/bin/date -s '" + this -> dateTimeFormated + "'";
    logTimeIsChanged();
    system(qPrintable(this -> dateTimeFormated));
    system(qPrintable("sudo /usr/sbin/hwclock -w"));
    stopUpdateTime = false;
}

void Settings::on_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
  if (stopUpdateTime)
  {
  this -> dateTimeNoFormated = dateTime.toString();
  this -> dateTimeFormated = dateTime.toString(format);
  }
}

void Settings::logTimeIsChanged()
{
    query.prepare("insert into log (Event, date, Time) values (:values, CURRENT_DATE(), CURRENT_TIME())");
    query.bindValue(":values","Дата и время изменены:");
    query.exec();

    query.prepare("insert into log (Event) values (:values)");
    query.bindValue(":values", this -> dateTimeNoFormated);
    query.exec();

    emit update_bd();

}



void Settings::on_ChangeSetting_clicked()
{

    if (passwordEnter())
    {
        menuIsActive();
    }
    else
    {
       menuIsInactive();
    }

}

void Settings::menuIsActive()
{
    ui -> label_13 -> setEnabled(true);
    ui -> label_2 -> setEnabled(true);
    ui -> pushButton_21 -> setEnabled(true);
    ui -> pushButton_16 -> setEnabled(true);
    ui -> pushButton_17 -> setEnabled(true);
    ui -> ChangePassword -> setEnabled(true);
    ui -> labelChangePassword -> setEnabled(true);
    ui -> pushButton_21 -> setEnabled(true);
    ui -> ChangeSetting -> setEnabled(false);
    ui -> Exit -> setEnabled(true);
    ui -> label_3 -> setEnabled(true);
    ui -> checkBox -> setEnabled(true);
    ui -> TakeAScreenWindow -> setEnabled(true);
    emit AlarmChangeSpinSetEnable(true);
}

void Settings::menuIsInactive()
{
    ui -> label_13 -> setEnabled(false);
    ui -> label_2 -> setEnabled(false);
    ui -> pushButton_16 -> setEnabled(false);
    ui -> pushButton_17 -> setEnabled(false);
    ui -> pushButton_18 -> setEnabled(false);
    ui -> pushButton_19 -> setEnabled(false);
    ui -> dateTimeEdit -> setEnabled(false);
    ui -> pushButton_21 -> setEnabled(false);
    ui -> ChangePassword -> setEnabled(false);
    ui -> labelChangePassword -> setEnabled(false);
    ui -> pushButton_21 -> setEnabled(false);
    ui -> ChangeSetting -> setEnabled(true);
    stopUpdateTime = false;
    ui -> Exit -> setEnabled(false);
    ui -> label_3 -> setEnabled(false);
    ui -> checkBox -> setEnabled(false);
    ui -> TakeAScreenWindow -> setEnabled(false);
    emit AlarmChangeSpinSetEnable(false);
}

bool Settings::passwordEnter()
{
    bool ok;
    QString text = QInputDialog :: getText (this, tr ("Введение пароля"),
                                            tr ("Для внесения изменений, пожалуйста, введите пароль"), QLineEdit :: Password, 0, &ok);
    QByteArray btext = text.toUtf8();
    if (ok and (QCryptographicHash::hash(btext, QCryptographicHash::Sha256).toHex()
                                        == adminPassword or
                QCryptographicHash::hash(btext, QCryptographicHash::Sha256).toHex()
                                                       == userPassword))
    {
        return true;
    }
    if (ok and !(QCryptographicHash::hash(btext, QCryptographicHash::Sha256).toHex()
                                        == adminPassword or
                QCryptographicHash::hash(btext, QCryptographicHash::Sha256).toHex()
                                                        == userPassword))
    {
        query.prepare("insert into log (Event, date, Time) values (:values, CURRENT_DATE(), CURRENT_TIME())");
        query.bindValue(":values","Неудачный ввод пароля");
        query.exec();
        emit update_bd();
       QMessageBox::warning (this, "Внимание","Пароль не верен!");
        return false;
    }

    return false;
}

void Settings::settingSensor(QFormLayout* temperetureSensorLayout, QLabel* SensorNameObjct, QString locationName,
                             QLabel* LabelLowerLimitAlarmSensor, QSpinBox* LowerLimitAlarmChangeSpin,
                             QLabel* LabelUpperLimitAlarmSensor, QSpinBox* UpperLimitAlarmChangeSpin,
                             QLabel* LabelUpperLimitWarningSensor, QSpinBox* UpperLimitWarningChangeSpin,
                             int number)
{
    temperetureSensorLayout = new QFormLayout (this);
    ui -> horizontalLayout_2 -> addLayout(temperetureSensorLayout);

    SensorNameObjct = new QLabel (this);
    SensorNameObjct -> setText(locationName);
    SensorNameObjct -> setEnabled(false);

    LabelLowerLimitAlarmSensor = new QLabel (this);
    LabelLowerLimitAlarmSensor->setText ("Нижний аварийный предел");
    LabelLowerLimitAlarmSensor->setEnabled(false);
    LowerLimitAlarmChangeSpin = new QSpinBox (this);
    LowerLimitAlarmChangeSpin->setEnabled(false);

    LabelUpperLimitAlarmSensor = new QLabel (this);
    LabelUpperLimitAlarmSensor->setText ("Верхний аварийный предел");
    LabelUpperLimitAlarmSensor->setEnabled(false);
    UpperLimitAlarmChangeSpin = new QSpinBox (this);
    UpperLimitAlarmChangeSpin->setEnabled(false);

    LabelUpperLimitWarningSensor = new QLabel (this);
    LabelUpperLimitWarningSensor->setEnabled(false);
    LabelUpperLimitWarningSensor->setText ("Верхний предел-предупреждение");
    UpperLimitWarningChangeSpin = new QSpinBox (this);
    UpperLimitWarningChangeSpin->setEnabled(false);


    connect (this, &Settings::AlarmChangeSpinSetEnable, SensorNameObjct, &QSpinBox::setEnabled);
    connect (this, &Settings::AlarmChangeSpinSetEnable, LowerLimitAlarmChangeSpin, &QSpinBox::setEnabled);
    connect (this, &Settings::AlarmChangeSpinSetEnable, LabelLowerLimitAlarmSensor, &QSpinBox::setEnabled);
    connect (this, &Settings::AlarmChangeSpinSetEnable, UpperLimitAlarmChangeSpin, &QSpinBox::setEnabled);
    connect (this, &Settings::AlarmChangeSpinSetEnable, LabelUpperLimitAlarmSensor, &QSpinBox::setEnabled);
    connect (this, &Settings::AlarmChangeSpinSetEnable, UpperLimitWarningChangeSpin, &QSpinBox::setEnabled);
    connect (this, &Settings::AlarmChangeSpinSetEnable, LabelUpperLimitWarningSensor, &QSpinBox::setEnabled);

    switch(number)
    {
    case 0:
    connect(LowerLimitAlarmChangeSpin, SIGNAL(valueChanged(int)),this, SLOT (LowerLimitAlarmBufferSlot1 (int)));
    connect (this, SIGNAL(ShowAlarmLowerLimit1 (int)),LowerLimitAlarmChangeSpin, SLOT (setValue(int)));

    connect(UpperLimitAlarmChangeSpin, SIGNAL(valueChanged(int)),this, SLOT (UpperLimitAlarmBufferSlot1 (int)));
    connect (this, SIGNAL(ShowAlarmUpperLimit1 (int)),UpperLimitAlarmChangeSpin, SLOT (setValue(int)));

    connect(UpperLimitWarningChangeSpin, SIGNAL(valueChanged(int)),this, SLOT (UpperLimitWarningBufferSlot1 (int)));
    connect (this, SIGNAL(ShowWarningUpperLimit1 (int)),UpperLimitWarningChangeSpin, SLOT (setValue(int)));
    break;


    case 1:
    connect(LowerLimitAlarmChangeSpin, SIGNAL(valueChanged(int)),this, SLOT (LowerLimitAlarmBufferSlot2 (int)));
    connect (this, SIGNAL(ShowAlarmLowerLimit2 (int)),LowerLimitAlarmChangeSpin, SLOT (setValue(int)));

    connect(UpperLimitAlarmChangeSpin, SIGNAL(valueChanged(int)),this, SLOT (UpperLimitAlarmBufferSlot2 (int)));
    connect (this, SIGNAL(ShowAlarmUpperLimit2 (int)),UpperLimitAlarmChangeSpin, SLOT (setValue(int)));

    connect(UpperLimitWarningChangeSpin, SIGNAL(valueChanged(int)),this, SLOT (UpperLimitWarningBufferSlot2 (int)));
    connect (this, SIGNAL(ShowWarningUpperLimit2 (int)),UpperLimitWarningChangeSpin, SLOT (setValue(int)));
    break;


    case 2:
    connect(LowerLimitAlarmChangeSpin, SIGNAL(valueChanged(int)),this, SLOT (LowerLimitAlarmBufferSlot3 (int)));
    connect (this, SIGNAL(ShowAlarmLowerLimit3 (int)),LowerLimitAlarmChangeSpin, SLOT (setValue(int)));

    connect(UpperLimitAlarmChangeSpin, SIGNAL(valueChanged(int)),this, SLOT (UpperLimitAlarmBufferSlot3 (int)));
    connect (this, SIGNAL(ShowAlarmUpperLimit3 (int)),UpperLimitAlarmChangeSpin, SLOT (setValue(int)));

    connect(UpperLimitWarningChangeSpin, SIGNAL(valueChanged(int)),this, SLOT (UpperLimitWarningBufferSlot3 (int)));
    connect (this, SIGNAL(ShowWarningUpperLimit3 (int)),UpperLimitWarningChangeSpin, SLOT (setValue(int)));
    break;
    }

    temperetureSensorLayout->addRow(SensorNameObjct);
    temperetureSensorLayout->addRow(LabelLowerLimitAlarmSensor, LowerLimitAlarmChangeSpin);
    temperetureSensorLayout->addRow(LabelUpperLimitAlarmSensor, UpperLimitAlarmChangeSpin);
    temperetureSensorLayout->addRow(LabelUpperLimitWarningSensor, UpperLimitWarningChangeSpin);

}

void Settings::on_Exit_clicked()
{

    menuIsInactive();
    on_pushButton_17_clicked();
    on_pushButton_19_clicked();

}

void Settings::on_ChangePassword_clicked()
{
     if (passwordEnter())
     {
         bool Exit = true;
         while (Exit)
            {
            QDialog dialog(this);
            QFormLayout form(&dialog);
            dialog.setWindowTitle("Изменение пароля");
            form.addRow (new QLabel("Ввод нового пароля"));
            QList<QLineEdit *> fields;
            QLineEdit *lineEdit1 = new QLineEdit(&dialog);
            QLineEdit *lineEdit2 = new QLineEdit(&dialog);
            QString label1 = QString("Введите пароль");
            QString label2 = QString("Повторите пароль");
            form.addRow(label1, lineEdit1);
            form.addRow(label2, lineEdit2);
            lineEdit1 -> setEchoMode (QLineEdit :: Password);
            lineEdit2 -> setEchoMode (QLineEdit :: Password);
            QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                    Qt::Horizontal, &dialog);
            form.addRow(&buttonBox);

            QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
            QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
            if (dialog.exec() == QDialog::Accepted)
                {
                if (lineEdit1->text() == lineEdit2->text())
                    {
                        QByteArray btext = lineEdit1->text().toUtf8();
                        userPassword = QCryptographicHash::hash(btext, QCryptographicHash::Sha256).toHex();

                        query.prepare("insert into log (Event, date, Time) values (:values, CURRENT_DATE(), CURRENT_TIME())");
                        query.bindValue(":values","Пароль от системы изменен");
                        query.exec();
                        emit update_bd();
                        saveSettingsToFile();
                        QMessageBox::information(this, "Пароль изменен","Пароль успешно изменен!");
                        Exit = false;
                     }
                    else
                     {
                        QMessageBox::warning (this, "Ошибка","Введенные пароли не совпадают");
                     }
                }
           else
            {
                 Exit = false;
            }
         }
       }
}

void Settings::on_checkBox_toggled(bool checked)
{
    emit TakeAPictureButtonVisibilitySignal(checked);
    checked==true?
    ui->TakeAPicture->show():
    ui->TakeAPicture->hide();
}

void Settings::on_TakeAPicture_clicked()
{
     ui->TakeAPicture->hide();
     QTimer *timer = new QTimer(this);
     timer->setSingleShot(true);
     connect (timer, &QTimer::timeout, [=](){
                          TakeAPictureSlot();
                          timer->deleteLater();
                                             });
     timer->start(50);
}

void Settings::TakeAPictureSlot()
{
    emit TakeAPictureSignal();
    ui->TakeAPicture->show();
}
