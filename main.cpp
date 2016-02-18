#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mainWnd;

    ///way to set res file in widgets
    /////////////////////////////////
    Q_INIT_RESOURCE(res);
    ////////////////////////////////

    mainWnd.setWindowTitle("Eco Test Creator");

    mainWnd.showStartView();

    return a.exec();
}
