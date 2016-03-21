#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "global.h"

#include <QWidget>
#include <QSystemTrayIcon>

class QPushButton;
class QVBoxLayout;
class TestCreatorView;
class SettingSubView;
class QSystemTrayIcon;
class QMenu;

class MainWindowTab;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

    void setVisible(bool visible) Q_DECL_OVERRIDE;

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

public slots:
    void showMainView(TestViews view);

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void messageClicked();
    void showMessage();

private:
    void hidePreviuosWindows();
    QRect getScreenGeometry() const;
    void createTrayIcon();
    void createActions();

    QAction *m_minimizeAction;
    QAction *m_maximizeAction;
    QAction *m_restoreAction;
    QAction *m_quitAction;

    MainWindowTab      *m_wnd;
    QSystemTrayIcon    *m_trayIcon;
    QMenu              *m_trayIconMenu;
};

#endif // MAINWINDOW_H
