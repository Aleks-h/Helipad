#include "mainwindow.h"

//Global var from config file//
int sensorsCount;            //
QString locationName1;       //
QString locationName2;       //
QString locationName3;       //
///////////////////////////////

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , address{"192.168.3.18"}
    , port{502} , errorWarning{false}
{
    db = new Database();
    db->ConnectToDatabase();

    CurStateInit(6);
    TCPModbus = new TCPModbusCommunication(CurStates);
    TCPModbusThread = new QThread(this);
    TCPModbus->moveToThread(TCPModbusThread);
    TCPModbusThread->start();
    connect(this, &MainWindow::startCommunication, TCPModbus, &TCPModbusCommunication::Connection, Qt::QueuedConnection);
    emit (startCommunication(address,502));



    window = new History_screen(this);

    initVariablesSubsys();
    initVariablesSensors();
    readConfigFromFile();

    settings = new Settings(this);
    connect(window, &History_screen::firstWindow, this, &MainWindow::showFullScreen);
    connect(settings, &Settings::firstWindow, this, &MainWindow::showFullScreen);
    connect(window, &History_screen::thirdWindow, settings, &Settings::showFullScreen);
    connect(settings, &Settings::secondWindow, window, &History_screen::showFullScreen);
    connect(this, &MainWindow::TakeAPicktureSignal, settings, &Settings::TakeAPictureSlot);
    connect(window, &History_screen::TakeAPicktureSignal, settings, &Settings::TakeAPictureSlot);

    connect(settings, &Settings::TakeAPictureButtonVisibilitySignal, this, &MainWindow::TakeAPictureButtonVisibilitySlot);
    connect(settings, &Settings::TakeAPictureButtonVisibilitySignal, window, &History_screen::TakeAPictureButtonVisibilitySlot);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    connect(timer,SIGNAL(timeout()),window,SLOT(timerUpdate1()));
    connect(timer,SIGNAL(timeout()),settings,SLOT(timerUpdate1()));
    timer->start(1000);
    connect(settings, SIGNAL(update_bd()), window, SLOT(SlotCreateModel()));

    connect(this, &MainWindow::update_bd, window, &History_screen::SlotCreateModel);
    connect(TCPModbus, &TCPModbusCommunication::logging, this, &MainWindow::logging);

    ui->setupUi(this);


    AlarmMassege = new QLabel(this);
    AlarmLayout = new QVBoxLayout(this);
    AlarmMassege->setStyleSheet("color: rgb(255, 0, 0); font-size: 30px");
    AlarmMassege->setAlignment(Qt::AlignCenter);
    AlarmMassege->setText("Нет связи с модулем управления ССО!");
    AlarmLayout->addWidget(AlarmMassege);
    ui->verticalLayout_3->addItem(AlarmLayout);


    ui->takeAPicture->hide();
//1////



    timer1 = new QTimer(this);
    timer1->start(1000);
    HSpItem=new QSpacerItem(630, 1, QSizePolicy::Maximum, QSizePolicy::Ignored);
    ui->horizontalLayout_3->addItem(HSpItem);



    for(int i=0; i<sensorsCount; i++)
    {
    sensors (sensor1.at(i), sensor2.at(i), sensor3.at(i), locationNameLabel.at(i), locationName.at(i), TemperLayout.at(i),
    TemperLayoutFull.at(i), FormLayout.at(i), sensorNameLabel1.at(i), sensorName1.at(i),
    sensorValue1.at(i), sensorNameLabel2.at(i), sensorName2.at(i),
             sensorValue2.at(i), sensorNameLabel3.at(i), sensorName3.at(i),
             sensorValue3.at(i), Frame.at(i), VSpItemTemp.at(i), unit1.at(i),
             unit2.at(i),unit3.at(i), i);
    }
     readSettingsFromFile();



    ////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////

    for(int i=0;  i < numberOfSubsystem; i++)
    {
        subsystem(VLay.at(i), VLayInd.at(i), VSpItem.at(i), Label.at(i),
                  PButOn.at(i), Indic.at(i), PButOff.at(i), name.at(i), &i);
    }

///////////////////////////////////////////////////////////////////////////////////////////////////

    ui->statusBar->showMessage("Москва, ООО 'Аеросвет', 2022");

}


MainWindow::~MainWindow()
{
    delete ui;
    delete TCPModbus;
    delete TCPModbusThread;
}

void MainWindow::on_pushButton_7_clicked()
{
 window->showFullScreen();
}


void MainWindow::on_pushButton_9_clicked()
{
    settings->showFullScreen();
}

void MainWindow::TakeAPictureButtonVisibilitySlot(bool & checked)
{
    checked==true?
    ui->takeAPicture->show():
    ui->takeAPicture->hide();
}

void MainWindow::sensors (TSensor* sensor1, TSensor* sensor2, TSensor* sensor3, QLabel* locationNameLabel, QString* locationName, QVBoxLayout* TemperLayout,
                         QVBoxLayout* TemperLayoutFull, QFormLayout* FormLayout,  QLabel* sensorNameLabel1, QString* sensorName1,
                        QLCDNumber* sensorValue1,   QLabel* sensorNameLabel2, QString* sensorName2,
                       QLCDNumber* sensorValue2,  QLabel* sensorNameLabel3, QString* sensorName3,
                       QLCDNumber* sensorValue3, QFrame* Frame, QSpacerItem* VSpItemTemp, QString* unit1, QString* unit2, QString* unit3, int numberSensor)

{

    locationNameLabel = new QLabel (this);
    locationNameLabel->setText(*locationName);
    locationNameLabel->setFrameStyle(QFrame::Box|QFrame::Raised);
    locationNameLabel->setAlignment(Qt::AlignTop|Qt::AlignHCenter);

    VSpItemTemp=new QSpacerItem (1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding);


    FormLayout = new QFormLayout(this);

    if (sensorName1!=QString(""))
    {
        sensor1 = new TSensor (*locationName, *sensorName1, *unit1);


        sensorNameLabel1 = new QLabel(this);
        sensorNameLabel1->setText(*sensorName1);
        sensorValue1 = new QLCDNumber(this);
        FormLayout->addRow(sensorNameLabel1, sensorValue1);

        //connect(timer,SIGNAL(timeout()),this,SLOT(sensorShow(TSensor &Sensor, QLCDNumber &Sensorvalue)));
        }
    if (sensorName2!=QString(""))
    {
        sensor2 = new TSensor (*locationName, *sensorName2, *unit2);
        sensorNameLabel2 = new QLabel(this);
        sensorNameLabel2->setText(*sensorName2);
        sensorValue2 = new QLCDNumber(this);
        FormLayout->addRow(sensorNameLabel2, sensorValue2);
    }

    if (sensorName3!=QString(""))
    {
        sensor3 = new TSensor (*locationName, *sensorName3, *unit3);
        sensorNameLabel3 = new QLabel(this);
        sensorNameLabel3->setText(*sensorName3);
        sensorValue3 = new QLCDNumber(this);
        FormLayout->addRow(sensorNameLabel3, sensorValue3);


        //connect(timer,SIGNAL(timeout()),this,SLOT(sensorShow(&Sensor, &Sensorvalue)));
    }


    switch (numberSensor)
    {
    case 0:
        connect(settings,SIGNAL(SetAlarmLowerLimit1 (int)),sensor1,SLOT(SetAlarmLowerLimit(int)));
        connect(settings,SIGNAL(SetAlarmUpperLimit1 (int)),sensor1,SLOT(SetAlarmUpperLimit(int)));
        connect(settings,SIGNAL(SetWarningUpperLimit1 (int)),sensor1,SLOT(SetWarningUpperLimit(int)));

        connect(settings,SIGNAL(CancelSetAlarmLimit ()),sensor1,SLOT(CancelSetAlarmLowerLimit()));
        connect(settings,SIGNAL(CancelSetAlarmLimit ()),sensor1,SLOT(CancelSetAlarmUpperLimit()));
        connect(settings,SIGNAL(CancelSetAlarmLimit ()),sensor1,SLOT(CancelSetWarningUpperLimit()));

        connect(sensor1, SIGNAL(ShowAlarmLowerLimitInSetting(int)),settings,SLOT(ShowLowerLimitAlarm1 (int)));
        connect(sensor1, SIGNAL(ShowAlarmUpperLimitInSetting(int)),settings,SLOT(ShowUpperLimitAlarm1 (int)));
        connect(sensor1, SIGNAL(ShowWarningUpperLimitInSetting(int)),settings,SLOT(ShowUpperLimitWarning1 (int)));

        connect(this, SIGNAL(SetAlarmLowerLimit1FromConfFile(int)),sensor1,SLOT(SetAlarmLowerLimit(int)));
        connect(this, SIGNAL(SetAlarmUpperLimit1FromConfFile(int)),sensor1,SLOT(SetAlarmUpperLimit (int)));
        connect(this, SIGNAL(SetWarningUpperLimit1FromConfFile(int)),sensor1,SLOT(SetWarningUpperLimit (int)));
    break;

    case 1:
        connect(settings,SIGNAL(SetAlarmLowerLimit2 (int)),sensor1,SLOT(SetAlarmLowerLimit(int)));
        connect(settings,SIGNAL(SetAlarmUpperLimit2 (int)),sensor1,SLOT(SetAlarmUpperLimit(int)));
        connect(settings,SIGNAL(SetWarningUpperLimit2 (int)),sensor1,SLOT(SetWarningUpperLimit(int)));

        connect(settings,SIGNAL(CancelSetAlarmLimit ()),sensor1,SLOT(CancelSetAlarmLowerLimit()));
        connect(settings,SIGNAL(CancelSetAlarmLimit ()),sensor1,SLOT(CancelSetAlarmUpperLimit()));
        connect(settings,SIGNAL(CancelSetAlarmLimit ()),sensor1,SLOT(CancelSetWarningUpperLimit()));

        connect(sensor1, SIGNAL(ShowAlarmLowerLimitInSetting(int)),settings,SLOT(ShowLowerLimitAlarm2 (int)));
        connect(sensor1, SIGNAL(ShowAlarmUpperLimitInSetting(int)),settings,SLOT(ShowUpperLimitAlarm2 (int)));
        connect(sensor1, SIGNAL(ShowWarningUpperLimitInSetting(int)),settings,SLOT(ShowUpperLimitWarning2 (int)));

        connect(this, SIGNAL(SetAlarmLowerLimit2FromConfFile(int)),sensor1,SLOT(SetAlarmLowerLimit(int)));
        connect(this, SIGNAL(SetAlarmUpperLimit2FromConfFile(int)),sensor1,SLOT(SetAlarmUpperLimit (int)));
        connect(this, SIGNAL(SetWarningUpperLimit2FromConfFile(int)),sensor1,SLOT(SetWarningUpperLimit (int)));
    break;

    case 2:
        connect(settings,SIGNAL(SetAlarmLowerLimit3 (int)),sensor1,SLOT(SetAlarmLowerLimit(int)));
        connect(settings,SIGNAL(SetAlarmUpperLimit3 (int)),sensor1,SLOT(SetAlarmUpperLimit(int)));
        connect(settings,SIGNAL(SetWarningUpperLimit3 (int)),sensor1,SLOT(SetWarningUpperLimit(int)));

        connect(settings,SIGNAL(CancelSetAlarmLimit ()),sensor1,SLOT(CancelSetAlarmLowerLimit()));
        connect(settings,SIGNAL(CancelSetAlarmLimit ()),sensor1,SLOT(CancelSetAlarmUpperLimit()));
        connect(settings,SIGNAL(CancelSetAlarmLimit ()),sensor1,SLOT(CancelSetWarningUpperLimit()));

        connect(sensor1, SIGNAL(ShowAlarmLowerLimitInSetting(int)),settings,SLOT(ShowLowerLimitAlarm3 (int)));
        connect(sensor1, SIGNAL(ShowAlarmUpperLimitInSetting(int)),settings,SLOT(ShowUpperLimitAlarm3 (int)));
        connect(sensor1, SIGNAL(ShowWarningUpperLimitInSetting(int)),settings,SLOT(ShowUpperLimitWarning3 (int)));

        connect(this, SIGNAL(SetAlarmLowerLimit3FromConfFile(int)),sensor1,SLOT(SetAlarmLowerLimit(int)));
        connect(this, SIGNAL(SetAlarmUpperLimit3FromConfFile(int)),sensor1,SLOT(SetAlarmUpperLimit (int)));
        connect(this, SIGNAL(SetWarningUpperLimit3FromConfFile(int)),sensor1,SLOT(SetWarningUpperLimit (int)));
    break;
    }

   connect(timer,SIGNAL(timeout()),this,SLOT(sensorShow(&Sensor, &Sensorvalue)));



    TemperLayout = new QVBoxLayout(this);

    Frame = new QFrame(this);
    TemperLayoutFull = new QVBoxLayout(this);
    TemperLayout->addWidget(locationNameLabel);
    TemperLayout->addLayout(FormLayout);
    TemperLayout->addItem(VSpItemTemp);
    Frame->setLayout(TemperLayout);
    Frame->setLineWidth(2);
    Frame->setFrameStyle(QFrame::Box|QFrame::Raised);
    TemperLayoutFull->addWidget(Frame);
    TemperLayoutFull->addItem(VSpItemTemp);
    ui->horizontalLayout_3->addLayout(TemperLayoutFull);

}



void MainWindow::readConfigFromFile()
{   
 QSettings settings("settingsExt.ini",QSettings::IniFormat);

 QTextCodec *codec = QTextCodec::codecForName("UTF-8");
 QTextCodec::setCodecForLocale(codec);
 settings.setIniCodec(codec);

 settings.beginGroup("subsystems");

    numberOfSubsystem = settings.value("subsystemCount").toInt();
    for (int i=1; i <= numberOfSubsystem; i++)
    {
    QString StrI = QString::number(i);
    settings.beginGroup(StrI);
    QString Qstr = settings.value("name").toString();
    *name.at(i-1) = Qstr;
    settings.endGroup();
    }
    settings.endGroup();

 settings.beginGroup("sensors");
    sensorsCount = settings.value("sensorsCount").toInt();
    for (int i=1; i <= sensorsCount; i++)
    {
    QString StrI = "name"+QString::number(i);
    QString Qstr = settings.value(StrI).toString();
    *locationName.at(i-1) = Qstr;
    settings.beginGroup(StrI);
    for(int j=1; j<=3; j++)
    {
        QString StrParam = QString::number(j);
        Qstr = settings.value(StrParam).toString();
        if(j==1)
        {
        *sensorName1.at(i-1) = Qstr;
        }
        if(j==2)
        {
        *sensorName2.at(i-1) = Qstr;
        }
        if(j==3)
        {
        *sensorName3.at(i-1) = Qstr;
        }
    }
    settings.endGroup();
    }


}

void MainWindow::readSettingsFromFile()
{
    QSettings settings("settings.ini",QSettings::IniFormat);
    settings.beginGroup("Settings");

    QString userPassword = settings.value("password").toString();
    QByteArray bUserPassword = userPassword.toUtf8();
    this -> settings -> setHashUserPassword (bUserPassword);
      settings.beginGroup("sensor1");
      int AlarmLowTempLimit1 = settings.value("AlarmLowTempLimit").toInt();
      int AlarmHighTempLimit1 = settings.value("AlarmHighTempLimit").toInt();
      int WarningHighLimit1 = settings.value("WarningHighTempLimit").toInt();

      emit SetAlarmLowerLimit1FromConfFile(AlarmLowTempLimit1);
      emit SetAlarmUpperLimit1FromConfFile(AlarmHighTempLimit1);
      emit SetWarningUpperLimit1FromConfFile(WarningHighLimit1);

      settings.endGroup();

      settings.beginGroup("sensor2");
      int AlarmLowTempLimit2 = settings.value("AlarmLowTempLimit").toInt();
      int AlarmHighTempLimit2 = settings.value("AlarmHighTempLimit").toInt();
      int WarningHighLimit2 = settings.value("WarningHighTempLimit").toInt();

      emit SetAlarmLowerLimit2FromConfFile(AlarmLowTempLimit2);
      emit SetAlarmUpperLimit2FromConfFile(AlarmHighTempLimit2);
      emit SetWarningUpperLimit2FromConfFile(WarningHighLimit2);

      settings.endGroup();

      settings.beginGroup("sensor3");
      int AlarmLowTempLimit3 = settings.value("AlarmLowTempLimit").toInt();
      int AlarmHighTempLimit3 = settings.value("AlarmHighTempLimit").toInt();
      int WarningHighLimit3 = settings.value("WarningHighTempLimit").toInt();

      emit SetAlarmLowerLimit3FromConfFile(AlarmLowTempLimit3);
      emit SetAlarmUpperLimit3FromConfFile(AlarmHighTempLimit3);
      emit SetWarningUpperLimit3FromConfFile(WarningHighLimit3);

      settings.endGroup();
      settings.endGroup();
}


void MainWindow::subsystem(QVBoxLayout* VLay, QVBoxLayout* VLayInd, QSpacerItem* VSpItem, QLabel* Label,
                           QPushButton* PButOn, QImageWidget* Indic, QPushButton* PButOff, QString* name, int* i)
{
    VLay = new QVBoxLayout (this);
    VLayInd = new QVBoxLayout (this);
    VSpItem = new QSpacerItem (10, 320, QSizePolicy::Maximum, QSizePolicy::Maximum);

    Label = new QLabel (this);
    Label->setText(*name);
    Label->setMinimumSize(150, 50);
    Label->setMaximumSize(150, 50);
    Label->setAlignment(Qt::AlignCenter);

    PButOn = new QPushButton (this);
    PButOn->setText("Вкл.");
    PButOn->setMinimumSize(150, 100);
    PButOn->setMaximumSize(150, 100);

    Indic = new QImageWidget (*name, *i, CurStates);
    Indic->setMinimumSize(150, 100);
    Indic->setMaximumSize(150, 100);


    PButOff = new QPushButton (this);
    PButOff->setText("Откл.");
    PButOff->setMinimumSize(150, 100);
    PButOff->setMaximumSize(150, 100);

    VLayInd->addWidget(Indic);

    VLay->addWidget(Label);
    VLay->addWidget(PButOn);
    VLay->addItem(VLayInd);
    VLay->addWidget(PButOff);
    VLay->addItem(VSpItem);

    connect(Indic, SIGNAL(update_bd()), window, SLOT(SlotCreateModel()));

    connect(Indic, &QImageWidget::writeValueSignal, TCPModbus, &TCPModbusCommunication::writeValue);

    connect(PButOn, &QPushButton::clicked, Indic, &QImageWidget::On);

    connect(PButOff, &QPushButton::clicked, Indic, &QImageWidget::Off);

    connect(this, &MainWindow::enableSignal, Indic, &QImageWidget::setEnabled);
    connect(this, &MainWindow::enableSignal, PButOff, &QPushButton::setEnabled);
    connect(this, &MainWindow::enableSignal, PButOn, &QPushButton::setEnabled);
    connect(this, &MainWindow::enableSignal, Label, &QLabel::setEnabled);

    ui->horizontalLayout->addLayout(VLay);

}

void MainWindow::timerUpdate()
{
    time = QDateTime::currentDateTime();

    str = time.toString("yyyy-MM-dd hh:mm:ss dddd");

    ui->time->setText(str);
}




void MainWindow::on_takeAPicture_clicked()
{
    ui->takeAPicture->hide();
    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);
    connect (timer, &QTimer::timeout, [=](){
                         TakeAPicture();
                         timer->deleteLater();
                                            });
    timer->start(50);

}

void MainWindow::TakeAPicture()
{
    emit TakeAPicktureSignal();
    ui->takeAPicture->show();
}

//-->
void MainWindow::sensorShow(TSensor &Sensor, QLCDNumber &Sensorvalue)
{
   Sensorvalue.display(Sensor.GetValues());
}

//-->
void MainWindow::sensorCheckLowerLimit()
{
  //  sensor1->CheckAlarmLowerLimit();
}

void MainWindow::CurStateInit(int numberOfSubsystem)
{
   QVector<bool> a(numberOfSubsystem);
   CurStates = a;
}


void MainWindow::logging(QString message)
{
    QSqlQuery query;
    query.prepare("insert into log (Event, date, Time) values (:name, CURRENT_DATE(), CURRENT_TIME());");
    query.bindValue(":name", message);
    query.exec();
    emit update_bd();
    if(message != "Модбас, соеденение установлено" and errorWarning == false)
    {
        errorWarning = true;
        QSound* play = new QSound (Path);
        play -> setLoops(QSound::Infinite);
        play -> play();
        massege = new QMessageBox;
        massege -> setText("Ошибка модбас соеденения");
        massege -> setDetailedText("Ошибка связи Modbus TCP");
        massege -> addButton(tr("Подтвердить"), QMessageBox::ActionRole);
        massege -> setIcon(QMessageBox::Critical);
        massege -> exec();
        delete massege;
        delete play;
        emit enableSignal(false);
        if (massege->clickedButton()) {

            QSqlQuery query;
            query.prepare("insert into log (Event, date, Time) values (:name, CURRENT_DATE(), CURRENT_TIME());");
            query.bindValue(":name","Ошибка модбас подтверждена");
            query.exec();
            emit update_bd();
            AlarmMassege->show();
         }
    }
        if(message == "Модбас, соеденение установлено")
            {
            AlarmMassege->hide();
            errorWarning = false;
            emit enableSignal(true);


            }
}

void MainWindow::initVariablesSubsys()
{

    VLay.push_back(VLay1);
    VLay.push_back(VLay2);
    VLay.push_back(VLay3);
    VLay.push_back(VLay4);
    VLay.push_back(VLay5);
    VLay.push_back(VLay6);
    VLay.push_back(VLay7);
    VLay.push_back(VLay8);
    VLay.push_back(VLay9);
    VLay.push_back(VLay10);

    VLayInd.push_back(VLayInd1);
    VLayInd.push_back(VLayInd2);
    VLayInd.push_back(VLayInd3);
    VLayInd.push_back(VLayInd4);
    VLayInd.push_back(VLayInd5);
    VLayInd.push_back(VLayInd6);
    VLayInd.push_back(VLayInd7);
    VLayInd.push_back(VLayInd8);
    VLayInd.push_back(VLayInd9);
    VLayInd.push_back(VLayInd10);

    VSpItem.push_back(VSpItem1);
    VSpItem.push_back(VSpItem2);
    VSpItem.push_back(VSpItem3);
    VSpItem.push_back(VSpItem4);
    VSpItem.push_back(VSpItem5);
    VSpItem.push_back(VSpItem6);
    VSpItem.push_back(VSpItem7);
    VSpItem.push_back(VSpItem8);
    VSpItem.push_back(VSpItem9);
    VSpItem.push_back(VSpItem10);

    Label.push_back(Label1);
    Label.push_back(Label2);
    Label.push_back(Label3);
    Label.push_back(Label4);
    Label.push_back(Label5);
    Label.push_back(Label6);
    Label.push_back(Label7);
    Label.push_back(Label8);
    Label.push_back(Label9);
    Label.push_back(Label10);

    PButOn.push_back(PButOn1);
    PButOn.push_back(PButOn2);
    PButOn.push_back(PButOn3);
    PButOn.push_back(PButOn4);
    PButOn.push_back(PButOn5);
    PButOn.push_back(PButOn6);
    PButOn.push_back(PButOn7);
    PButOn.push_back(PButOn8);
    PButOn.push_back(PButOn9);
    PButOn.push_back(PButOn10);

    Indic.push_back(Indic1);
    Indic.push_back(Indic2);
    Indic.push_back(Indic3);
    Indic.push_back(Indic4);
    Indic.push_back(Indic5);
    Indic.push_back(Indic6);
    Indic.push_back(Indic7);
    Indic.push_back(Indic8);
    Indic.push_back(Indic9);
    Indic.push_back(Indic10);

    PButOff.push_back(PButOff1);
    PButOff.push_back(PButOff2);
    PButOff.push_back(PButOff3);
    PButOff.push_back(PButOff4);
    PButOff.push_back(PButOff5);
    PButOff.push_back(PButOff6);
    PButOff.push_back(PButOff7);
    PButOff.push_back(PButOff8);
    PButOff.push_back(PButOff9);
    PButOff.push_back(PButOff10);

    name.push_back(&name1);
    name.push_back(&name2);
    name.push_back(&name3);
    name.push_back(&name4);
    name.push_back(&name5);
    name.push_back(&name6);
    name.push_back(&name7);
    name.push_back(&name8);
    name.push_back(&name9);
    name.push_back(&name10);
}

void MainWindow::initVariablesSensors()
{

    sensor1.push_back(sensor1_1);
    sensor1.push_back(sensor1_2);
    sensor1.push_back(sensor1_3);

    sensor2.push_back(sensor2_1);
    sensor2.push_back(sensor2_2);
    sensor2.push_back(sensor2_3);

    sensor3.push_back(sensor3_1);
    sensor3.push_back(sensor3_2);
    sensor3.push_back(sensor3_3);

    locationNameLabel.push_back(locationNameLabel1);
    locationNameLabel.push_back(locationNameLabel2);
    locationNameLabel.push_back(locationNameLabel3);

    locationName.push_back(&locationName1);
    locationName.push_back(&locationName2);
    locationName.push_back(&locationName3);

    TemperLayout.push_back(TemperLayout1);
    TemperLayout.push_back(TemperLayout2);
    TemperLayout.push_back(TemperLayout3);

    TemperLayoutFull.push_back(TemperLayoutFull1);
    TemperLayoutFull.push_back(TemperLayoutFull2);
    TemperLayoutFull.push_back(TemperLayoutFull3);

    FormLayout.push_back(FormLayout1);
    FormLayout.push_back(FormLayout2);
    FormLayout.push_back(FormLayout3);

    Frame.push_back(Frame1);
    Frame.push_back(Frame2);
    Frame.push_back(Frame3);

    VSpItemTemp.push_back(VSpItemTemp1);
    VSpItemTemp.push_back(VSpItemTemp2);
    VSpItemTemp.push_back(VSpItemTemp3);

    unit1.push_back(&unit1_1);
    unit1.push_back(&unit1_2);
    unit1.push_back(&unit1_3);

    unit2.push_back(&unit2_1);
    unit2.push_back(&unit2_2);
    unit2.push_back(&unit2_3);

    unit3.push_back(&unit3_1);
    unit3.push_back(&unit3_2);
    unit3.push_back(&unit3_3);

    sensorNameLabel1.push_back(sensorNameLabel1_1);
    sensorNameLabel1.push_back(sensorNameLabel1_2);
    sensorNameLabel1.push_back(sensorNameLabel1_3);

    sensorNameLabel2.push_back(sensorNameLabel2_1);
    sensorNameLabel2.push_back(sensorNameLabel2_2);
    sensorNameLabel2.push_back(sensorNameLabel2_3);

    sensorNameLabel3.push_back(sensorNameLabel3_1);
    sensorNameLabel3.push_back(sensorNameLabel3_2);
    sensorNameLabel3.push_back(sensorNameLabel3_3);

    sensorName1.push_back(&sensorName1_1);
    sensorName1.push_back(&sensorName1_2);
    sensorName1.push_back(&sensorName1_3);

    sensorName2.push_back(&sensorName2_1);
    sensorName2.push_back(&sensorName2_2);
    sensorName2.push_back(&sensorName2_3);

    sensorName3.push_back(&sensorName3_1);
    sensorName3.push_back(&sensorName3_2);
    sensorName3.push_back(&sensorName3_3);

    sensorValue1.push_back(sensorValue1_1);
    sensorValue1.push_back(sensorValue1_2);
    sensorValue1.push_back(sensorValue1_3);

    sensorValue2.push_back(sensorValue2_1);
    sensorValue2.push_back(sensorValue2_2);
    sensorValue2.push_back(sensorValue2_3);

    sensorValue3.push_back(sensorValue3_1);
    sensorValue3.push_back(sensorValue3_2);
    sensorValue3.push_back(sensorValue3_3);

}

