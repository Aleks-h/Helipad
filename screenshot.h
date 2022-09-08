#ifndef SCREENSHOT_H
#define SCREENSHOT_H
#include <QMainWindow>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

class screenShot : public QMainWindow
{
    Q_OBJECT
public:
    screenShot(QWidget *parent = nullptr);

private:
    void createTable();

private slots:
  //  void slotCurrentPicture();
  //  void slotTakeAPicture();
};

#endif // SCREENSHOT_H
