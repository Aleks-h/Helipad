#ifndef SCREEENSHOTWINDOW_H
#define SCREEENSHOTWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPixmap>
#include <QAbstractItemView>
#include <QSqlTableModel>
#include <QBuffer>
#include <QScreen>
#include <QDateTime>
#include <QThread>
#include <QTimer>

#include "screenshot.h"

namespace Ui {
class screeenShotWindow;
}

class screeenShotWindow : public QMainWindow
{
    Q_OBJECT



public:
    explicit screeenShotWindow(QWidget *parent = nullptr);
    ~screeenShotWindow();

     bool insertIntoTable(const QString &name, const QByteArray &pic);
     bool insertIntoTable(const QVariantList &data);

signals:
     void exit();

public slots:

      void slotCurrentPicture(QModelIndex);
      void on_TakeAPick_clicked();
private slots:


  //   void on_pushButton_toggled(bool checked);

     void on_Exit_clicked();

private:
    QSqlTableModel* model;
    Ui::screeenShotWindow *ui;

    void creatTabelModel();
    void createTable();
    void TakeAPicture();
};

#endif // SCREEENSHOTWINDOW_H
