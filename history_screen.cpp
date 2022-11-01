#include "history_screen.h"
#include "ui_history_screen.h"
#include "mainwindow.h"
#include "QDebug"
#include "QTableView"
#include <QTimer>

History_screen::History_screen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::History_screen)
{
   ui->setupUi(this);
   model = new QSqlTableModel;
   this ->createModel(QStringList() << tr("Событие")
                   << tr("Дата")
                   << tr("Время"));

   ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
   ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
   ui->tableView->horizontalHeader()->setStretchLastSection(true);
   ui->tableView->setUpdatesEnabled(true);
   ui->tableView->setModel(model);
   ui->tableView->scrollToBottom();
   ui->statusbar->showMessage("Москва, ООО 'Аеросвет', 2022");

   ui->TakeAPicture->hide();

   WidgetLayout(1.28, 1, 1.075);

   //widget.move(rect.width()/2-widget.with()/2,rect.height()/2-widget.heigth()/2);


}

void History_screen::WidgetLayout(double widthCof, double heightUpCof, double heightButtomCof)
{
    QScreen* screen = QApplication::screens().at(1);
    int width = screen->availableSize().width();
    int height = screen->availableSize().height();
    int width2 = width/widthCof;
    int height2h = height/heightUpCof;
    int height2l = height/heightButtomCof;
    ui->horizontalLayout_6->setContentsMargins (width-width2, (height-height2h)+(height/10), width-width2, height-height2l);
}

void History_screen::createModel(const QStringList &header)
{
    model->setTable("log");
    for ( int i = 0; i < header.count(); i++)
    {
    model->setHeaderData(i,Qt::Horizontal, header[i]);
    model->select();
    }
}


History_screen::~History_screen()
{
    delete ui;
}

void History_screen::on_pushButton_15_clicked()
{
    this->close();      // Закрываем окно
    emit firstWindow();
}

void History_screen::SlotCreateModel()
{
    this ->createModel(QStringList() << tr("Событие")
                    << tr("Дата")
                    << tr("Время"));
    ui->tableView->scrollToBottom();
}

void History_screen::timerUpdate1()
{
    time = QDateTime::currentDateTime();

    str = time.toString("yyyy-MM-dd hh:mm:ss dddd");

     ui->time->setText(str);
}


void History_screen::on_pushButton_17_clicked()
{
  this->close();
  emit thirdWindow();
}

void History_screen::on_TakeAPicture_clicked()
{
    ui->TakeAPicture->hide();
    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);
    connect (timer, &QTimer::timeout, [=](){
                         TakeAPicture();
                         timer->deleteLater();
                                            });
    timer->start(50);
}

void History_screen::TakeAPictureButtonVisibilitySlot(bool & checked)
{
    checked==true?
    ui->TakeAPicture->show():
    ui->TakeAPicture->hide();
}


void History_screen::TakeAPicture()
{
    emit TakeAPicktureSignal();
    ui->TakeAPicture->show();
}
