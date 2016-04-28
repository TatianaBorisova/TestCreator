#include "mainwindow.h"

#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Ошибка"),
                              QObject::tr("Не могу найти трей на данной системе"));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);

    // For correct encoding
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

    QTextCodec::setCodecForLocale(codec);

    MainWindow mainWnd;

    mainWnd.setWindowTitle("Eco Test Creator");

    mainWnd.showMainView(StartView);

    return a.exec();
}
