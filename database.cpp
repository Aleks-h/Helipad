#include "database.h"
#include <QDebug>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include "mainwindow.h"

Database::Database()
{
}

void Database::ConnectToDatabase()
{        
        qDebug()<<"Соеденение с базой данных...";
        db = QSqlDatabase::addDatabase ("QMYSQL");
            db.setHostName("localhost");
            db.setDatabaseName("Helipad");
            db.setUserName("user");
            db.setPassword("232323");
            if (db.open())
            {qDebug()<<"Соеденение с базой данных успешно установлено";}
            else
            {qDebug()<<"Ошибка соеденения c базой данных";}

}

void Database::closeDataBase()
{
    db.close();
}
