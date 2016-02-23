#include "mainwindow.h"
#include "views/mainwindowtab.h"

#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QPushButton>
#include <QApplication>
#include <QRect>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    //    m_vbox(new QVBoxLayout(this)),
    //    m_settingsBtn(new QPushButton(this)),
    //    m_testCreatorBtn(new QPushButton(this)),
    //    m_testCreatorView(new TestCreatorView(this)),
    //    m_serverSettingsView(new ServerSettingsView(this)),
    m_wnd(new MainWindowTab(this))
{
    setFixedSize(getScreenGeometry().width()*0.9, getScreenGeometry().height()*0.9);

    connect(m_wnd, &MainWindowTab::showView, this, &MainWindow::showMainView);

    //    connect(m_testCreatorBtn, &QPushButton::clicked, this, &MainWindow::showTestCreatorView);
    //    connect(m_settingsBtn,    &QPushButton::clicked, this, &MainWindow::showSettingsView);
    //    connect(m_testCreatorView, &TestCreatorView::backPressed, this, &MainWindow::showStartView);
    //    connect(m_serverSettingsView, &ServerSettingsView::backToPreviousView, this, &MainWindow::showStartView);

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

//void MainWindow::showSettingsView()
//{
//    hidePreviuosWindows();
//    this->move(getScreenGeometry().width()*0.2, getScreenGeometry().height()*0.2);

//    setFixedSize(500, 300);
//    m_serverSettingsView->show();
//    this->show();
//}

//void MainWindow::showTestCreatorView()
//{
//    hidePreviuosWindows();
//    this->move(0, 0);

//    setFixedSize(getScreenGeometry().width()*0.9, getScreenGeometry().height()*0.9);
//    m_testCreatorView->setFixedSize(getScreenGeometry().width()*0.9, getScreenGeometry().height()*0.9);
//    m_testCreatorView->resetElementsSize();
//    m_testCreatorView->show();
//    this->show();
//}
