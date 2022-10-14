#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlTableModel>
#include <QDebug>




class Database
{
public:
    Database ();
    void ConnectToDatabase();
    void log (const QString);
    QSqlDatabase db;
    void closeDataBase();
private:

};

#endif // DATABASE_H
