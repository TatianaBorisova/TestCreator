#ifndef MAINWINDOWTAB_H
#define MAINWINDOWTAB_H

#include "testcreatorbaseview.h"

#include <QWidget>

class QTabWidget;
class QGridLayout;
class QuestionCreatorTabView;
class SettingsTabView;
class TestDbView;
class ResultDbView;

class MainWindowTab : public TestCreatorBaseView
{
    Q_OBJECT
public:
    explicit MainWindowTab(QWidget *parent = 0);

    void setCurrentTabView(TestViews view);

protected:
    virtual void resize();

private:
    QTabWidget  *m_tab;
    QGridLayout *m_box;

    QuestionCreatorTabView *m_creatorPage;
    SettingsTabView        *m_settingsPage;
    TestDbView             *m_createdTestPage;
    ResultDbView           *m_testResultPage;
};

#endif // MAINWINDOWTAB_H
