#include "logging.h"
#include "ui_History.h"

Logging::Logging(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

};

Logging::~Logging()
{
    delete ui;
};
