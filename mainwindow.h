#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QPushButton;
class QVBoxLayout;
class TestCreatorView;
class ServerSettingsView;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

public slots:
    void showSettingsView();
    void showTestCreatorView();
    void showStartView();

private:
    void hidePreviuosWindows();
    QRect getScreenGeometry() const;

    QVBoxLayout        *m_vbox;
    QPushButton        *m_settingsBtn;
    QPushButton        *m_testCreatorBtn;
    TestCreatorView    *m_testCreatorView;
    ServerSettingsView *m_serverSettingsView;
};

#endif // MAINWINDOW_H
