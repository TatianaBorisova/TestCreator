#include "mainwindow.h"
#include "testcreatorview.h"
#include "serversettingsview.h"

#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QPushButton>
#include <QApplication>
#include <QRect>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    m_vbox(new QVBoxLayout(this)),
    m_settingsBtn(new QPushButton(this)),
    m_testCreatorBtn(new QPushButton(this)),
    m_testCreatorView(new TestCreatorView(this)),
    m_serverSettingsView(new ServerSettingsView(this))
{
    connect(m_testCreatorBtn, &QPushButton::clicked, this, &MainWindow::showTestCreatorView);
    connect(m_settingsBtn,    &QPushButton::clicked, this, &MainWindow::showSettingsView);
    connect(m_testCreatorView, &TestCreatorView::backPressed, this, &MainWindow::showStartView);
    connect(m_serverSettingsView, &ServerSettingsView::backToPreviousView, this, &MainWindow::showStartView);

    hidePreviuosWindows();
}

void MainWindow::showSettingsView()
{
    hidePreviuosWindows();
    this->move(getScreenGeometry().width()*0.2, getScreenGeometry().height()*0.2);

    setFixedSize(500, 300);
    m_serverSettingsView->show();
    this->show();
}

//private
QRect MainWindow::getScreenGeometry() const
{
    return QApplication::desktop()->screenGeometry();
}

void MainWindow::showTestCreatorView()
{
    hidePreviuosWindows();
    this->move(0, 0);

    setFixedSize(getScreenGeometry().width()*0.9, getScreenGeometry().height()*0.9);
    m_testCreatorView->setFixedSize(getScreenGeometry().width()*0.9, getScreenGeometry().height()*0.9);
    m_testCreatorView->resetElementsSize();
    m_testCreatorView->show();
    this->show();
}

void MainWindow::showStartView()
{
    hidePreviuosWindows();

    this->move(getScreenGeometry().width()*0.2, getScreenGeometry().height()*0.2);
    setFixedSize(300, 200);

    m_settingsBtn->setVisible(true);
    m_testCreatorBtn->setVisible(true);

    m_settingsBtn->setFixedSize(120, 50);
    m_testCreatorBtn->setFixedSize(120, 50);

    QFont font("Times", 14);
    m_settingsBtn->setFont(font);
    m_testCreatorBtn->setFont(font);

    m_settingsBtn->setText("Настройки");
    m_testCreatorBtn->setText("Создать тест");

    m_vbox->addWidget(m_settingsBtn);
    m_vbox->addWidget(m_testCreatorBtn);

    m_vbox->setContentsMargins((300 - 120)/2, 10, 10, 10);

    setLayout(m_vbox);

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
