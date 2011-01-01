#include <QtGui/QApplication>
#include <QtCore/QtPlugin>
#include "mainwindow.h"

Q_IMPORT_PLUGIN(qtiff)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
