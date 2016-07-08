#include "mainwindow.h"
#include <QApplication>
#include <QLocale>

int main(int argc, char *argv[])
{

    //QLocale locale(QLocale::Russian, QLocale::Russia);
    //QLocale::setDefault(locale);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
