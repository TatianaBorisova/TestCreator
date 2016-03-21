#include "mainwindow.h"

#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"),
                              QObject::tr("I couldn't detect any system tray "
                                          "on this system."));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);

    MainWindow mainWnd;

    mainWnd.setWindowTitle("Eco Test Creator");

    mainWnd.showMainView(StartView);

    return a.exec();
}
