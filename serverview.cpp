#include "serverview.h"
#include "tcpserver.h"

#include <QPushButton>
#include <QHBoxLayout>

ServerView::ServerView(QWidget *parent) :
    QWidget(parent),
    m_hbox(new QHBoxLayout(this)),
    m_server(new TcpServer(this)),
    m_startBtn(new QPushButton(this)),
    m_stopBtn(new QPushButton(this))
{
    connect(m_startBtn, &QPushButton::clicked, m_server, &TcpServer::startServer);
    connect(m_stopBtn, &QPushButton::clicked, m_server, &TcpServer::stopServer);

    setFixedSize(400, 200);

    setContentsMargins(0, 0, 0, 0);

    QFont btnFont;
    btnFont.setPixelSize(14);

    m_startBtn->setFixedSize(150, 50);
    m_stopBtn->setFixedSize(150, 50);

    m_startBtn->setFont(btnFont);
    m_stopBtn->setFont(btnFont);

    m_startBtn->setText("Запустить сервер");
    m_stopBtn->setText("Остановить сервер");

    m_hbox->setMargin(10);
    m_hbox->addWidget(m_startBtn);
    m_hbox->addWidget(m_stopBtn);

    this->setLayout(m_hbox);
}

