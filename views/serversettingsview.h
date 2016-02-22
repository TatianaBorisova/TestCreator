#ifndef SERVERSETTINGSVIEW_H
#define SERVERSETTINGSVIEW_H

#include <QWidget>

class QGridLayout;
class TcpServer;
class QPushButton;
class QLabel;
class QLineEdit;

class ServerSettingsView : public QWidget
{
    Q_OBJECT
public:
    explicit ServerSettingsView(QWidget *parent = 0);
    void startServer();

signals:
    void backToPreviousView();

private:
    QGridLayout *m_grid;
    TcpServer   *m_server;
    QLabel      *m_localhost;
    QLineEdit   *m_port;
    QPushButton *m_startBtn;
    QPushButton *m_stopBtn;
    QPushButton *m_backBtn;
};

#endif // SERVERSETTINGSVIEW_H
