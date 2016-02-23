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
    void showMainView(TestViews view);

private:
    void hidePreviuosWindows();
    QRect getScreenGeometry() const;

    MainWindowTab      *m_wnd;
};

#endif // MAINWINDOW_H
