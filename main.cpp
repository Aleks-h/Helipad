#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <./database.h>
#include <QTranslator>
#include <QLibraryInfo>



int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

   QTranslator qtTranslator;
    if (qtTranslator.load(QLocale::Russian,
                "qt", "_",
                QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
    {
        qDebug() << "qtTranslator ok";
        a.installTranslator(&qtTranslator);
    }
    MainWindow w;
    w.showFullScreen();
   return a.exec();
}




