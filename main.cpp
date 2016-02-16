#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mainWnd;

    mainWnd.setWindowTitle("Eco Test Creator");

    mainWnd.showStartView();

    return a.exec();
}
