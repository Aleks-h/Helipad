#ifndef HISTORY_SCREEN_H
#define HISTORY_SCREEN_H
#include <QSqlTableModel>
#include <QMainWindow>
#include <QMainWindow>
#include <database.h>
#include <QDateTime>


#include "screenshotscreen.h"


namespace Ui {
class History_screen;
}

class History_screen : public QMainWindow{
    Q_OBJECT
public:
    explicit History_screen(QWidget *parent = nullptr);
    ~History_screen();
    Database   *db;

private slots:
    void on_pushButton_15_clicked();
    void createModel(const QStringList &header);
    void timerUpdate1();
    void on_pushButton_17_clicked();

private:
     Ui::History_screen *ui;
     QSqlTableModel  *model;


     QString str;
     QDateTime time;

signals:
     void firstWindow();
     void thirdWindow();

public slots:
     void SlotCreateModel();

};

#endif // HISTORY_SCREEN_H
