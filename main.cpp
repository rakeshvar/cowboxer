#include <QtGui/QApplication>
#include <QtCore/QtPlugin>
#include "mainwindow.h"

#ifdef WIN32
Q_IMPORT_PLUGIN(qtiff)
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
