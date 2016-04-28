#include "mainwindow.h"
#include "views/mainwindowtab.h"

#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QPushButton>
#include <QApplication>
#include <QRect>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    TestCreatorBaseView(parent),
    m_wnd(new MainWindowTab(this)),
    m_trayIcon(new QSystemTrayIcon(this)),
    m_trayIconMenu(new QMenu(this)),
    m_screenSettings(new QSettings("resolution.ini", QSettings::IniFormat))
{
    m_screenSettings->beginGroup("mainscreen");
    int mainW = m_screenSettings->value("width").toInt();
    int mainH = m_screenSettings->value("height").toInt();
    int x = m_screenSettings->value("x_coord").toInt();
    int y = m_screenSettings->value("y_coord").toInt();
    m_screenSettings->endGroup();

    if(mainW !=0 && mainH != 0)
        setGeometry(x, y, mainW, mainH);
    else
        setGeometry((getScreenGeometry().width()*0.02)/2, (getScreenGeometry().height()*0.1)/2, getScreenGeometry().width()*0.98, getScreenGeometry().height()*0.9);

    setMinimumSize(width()/2, height()/2);

    m_wnd->setFixedSize(width(), height());

    connect(m_wnd, &MainWindowTab::showView, this, &MainWindow::showMainView);

    this->setStyleSheet("font-family: Arial; font-style: normal; font-size: 13pt;");

    connect(m_trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    hidePreviuosWindows();
    createActions();
    createTrayIcon();
}

//private
QRect MainWindow::getScreenGeometry() const
{
    return QApplication::desktop()->screenGeometry();
}

void MainWindow::showMainView(TestViews view)
{
    hidePreviuosWindows();
    m_wnd->setCurrentTabView(view);
    m_wnd->show();
    this->show();
    //this->showFullScreen();
}

void MainWindow::hidePreviuosWindows()
{
    QObjectList children = this->children();
    for (int i = 0; i < children.count(); i++) {
        QWidget *child = dynamic_cast<QWidget*>(children.at(i));
        if (child) {
            child->hide();
        }
    }
}

void MainWindow::createTrayIcon()
{
    QIcon icon(":res/test.png");
    m_trayIcon->setIcon(icon);
    setWindowIcon(icon);

    m_trayIcon->setToolTip("Test Creator");

    m_trayIconMenu->addAction(m_minimizeAction);
    m_trayIconMenu->addAction(m_maximizeAction);
    m_trayIconMenu->addAction(m_restoreAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(m_quitAction);

    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_trayIcon->show();
}

void MainWindow::createActions()
{
    m_minimizeAction = new QAction(tr("Свернуть"), this);
    connect(m_minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    m_maximizeAction = new QAction(tr("Развернуть"), this);
    connect(m_maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    m_restoreAction = new QAction(tr("Востановить окно"), this);
    connect(m_restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    m_quitAction = new QAction(tr("Выход"), this);
    connect(m_quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_trayIcon->isVisible()) {
        QMessageBox::information(this, tr("Информационное окно"),
                                 tr("Программа продолжит работать и будет доступна из "
                                    "системного трея.\nДля выхода из программы "
                                    "выберите Выход в контекстном меню "
                                    "системного трея."));
        hide();
        event->ignore();
    }
}

void MainWindow::setVisible(bool visible)
{
    m_minimizeAction->setEnabled(visible);
    m_maximizeAction->setEnabled(!visible);
    m_restoreAction->setEnabled(!visible);
    QWidget::setVisible(visible);
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    //case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        showMaximized();
        break;
    case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;
    default:
        break;
    }
}

void MainWindow::messageClicked()
{
    QMessageBox::information(0, tr("Test Creator"),  tr("Test Creator application."));
}

void MainWindow::showMessage()
{
    m_trayIcon->showMessage("Information", "Test Creator приложение запущено.", QSystemTrayIcon::Information, 5000);
}

void MainWindow::resize()
{
    m_wnd->setFixedSize(width(), height());
}
