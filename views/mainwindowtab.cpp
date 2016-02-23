#include "mainwindowtab.h"
#include "questioncreatortabview.h"
#include "settingstabview.h"
#include "testdbview.h"
#include "resultdbview.h"

#include <QTabWidget>
#include <QGridLayout>

MainWindowTab::MainWindowTab(QWidget *parent) :
    TestCreatorBaseView(parent),
    m_tab(new QTabWidget(this)),
    m_box(new QGridLayout(this)),
    m_creatorPage(new QuestionCreatorTabView(this)),
    m_settingsPage(new SettingsTabView(this)),
    m_createdTestPage(new TestDbView(this)),
    m_testResultPage(new ResultDbView(this))
{
    connect(m_createdTestPage, &TestDbView::showView, this, &MainWindowTab::showView);
    connect(m_settingsPage, &SettingsTabView::testDbChanged, m_creatorPage, &QuestionCreatorTabView::testDbChanged);

    m_tab->addTab(m_creatorPage, "Создать тест");
    m_tab->addTab(m_settingsPage, "Настройки");
    m_tab->addTab(m_createdTestPage, "Тесты");
    m_tab->addTab(m_testResultPage, "Результаты тестов");

    m_box->addWidget(m_tab);
    setLayout(m_box);
}

void MainWindowTab::resize()
{
    QWidget *wParent = dynamic_cast<QWidget *>(parent());
    if (wParent) {
        setFixedSize(wParent->width(), wParent->height());
    }

    m_creatorPage->setFixedSize(width()*0.9, height()*0.9);
    m_settingsPage->setFixedSize(width()*0.9, height()*0.9);
    m_createdTestPage->setFixedSize(width()*0.9, height()*0.9);
    m_testResultPage->setFixedSize(width()*0.9, height()*0.9);
}

void MainWindowTab::setCurrentTabView(TestViews view)
{
    m_tab->setCurrentIndex(view);
}
