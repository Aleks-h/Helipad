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
<<<<<<< HEAD
=======

<<<<<<< HEAD
>>>>>>> parent of 85dd171 (git add)
=======
>>>>>>> parent of 85dd171 (git add)
};

#endif // DATABASE_H
