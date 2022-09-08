#include "screenshot.h"

screenShot::screenShot(QWidget *parent):
       QMainWindow(parent)
{
    createTable();
}


void screenShot::createTable()
{
    QSqlQuery query;
        if(!query.exec( "CREATE TABLE screenDB ( \
                        id MEDIUMINT NOT NULL AUTO_INCREMENT, \
                        PRIMARY KEY (id), \
                        Name VARCHAR(255) NOT NULL, \
                        Picture BLOB (120000) NOT NULL \
                     );"
                       ))
            qDebug() << query.lastError().text();
}
