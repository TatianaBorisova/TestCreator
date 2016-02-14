#ifndef SERVERVIEW_H
#define SERVERVIEW_H

#include <QWidget>

class QHBoxLayout;
class TcpServer;
class QPushButton;

class ServerView : public QWidget
{
    Q_OBJECT
public:
    explicit ServerView(QWidget *parent = 0);

private:
    QHBoxLayout  *m_hbox;
    TcpServer *m_server;
    QPushButton  *m_startBtn;
    QPushButton  *m_stopBtn;
};

#endif // SERVERVIEW_H
