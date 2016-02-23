#include "mainwindow.h"
#include "views/mainwindowtab.h"

#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QPushButton>
#include <QApplication>
#include <QRect>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    m_wnd(new MainWindowTab(this))
{
    setFixedSize(getScreenGeometry().width()*0.9, getScreenGeometry().height()*0.9);

    connect(m_wnd, &MainWindowTab::showView, this, &MainWindow::showMainView);

    this->setStyleSheet("font-family: Arial; font-style: normal; font-size: 13pt;");

    hidePreviuosWindows();
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
