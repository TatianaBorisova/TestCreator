#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "global.h"

#include <QWidget>

class QPushButton;
class QVBoxLayout;
class TestCreatorView;
class SettingSubView;

class MainWindowTab;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

public slots:
//    void showSettingsView();
//    void showTestCreatorView();
    void showMainView(TestViews view);

private:
    void hidePreviuosWindows();
    QRect getScreenGeometry() const;

//    QVBoxLayout        *m_vbox;
//    QPushButton        *m_settingsBtn;
//    QPushButton        *m_testCreatorBtn;
//    TestCreatorView    *m_testCreatorView;
//    ServerSettingsView *m_serverSettingsView;
    MainWindowTab      *m_wnd;
};

#endif // MAINWINDOW_H
