#include "serversettingsview.h"
#include "tcpserver.h"

#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

namespace {
const int btnWidth = 150;
const int btnHeight = 50;
}

ServerSettingsView::ServerSettingsView(QWidget *parent) :
    QWidget(parent),
    m_grid(new QGridLayout(this)),
    m_server(new TcpServer(this)),
    m_localhost(new QLabel(this)),
    m_port(new QLineEdit(this)),
    m_startBtn(new QPushButton(this)),
    m_stopBtn(new QPushButton(this)),
    m_backBtn(new QPushButton(this))
{
    setFixedSize(500, 300);

    connect(m_startBtn, &QPushButton::clicked, this, &ServerSettingsView::startServer);
    connect(m_stopBtn,  &QPushButton::clicked, m_server, &TcpServer::stopServer);
    connect(m_backBtn,  &QPushButton::clicked, this, &ServerSettingsView::backToPreviousView);

    QFont btnFont;
    btnFont.setPixelSize(14);

    m_startBtn->setFixedSize(btnWidth, btnHeight);
    m_stopBtn->setFixedSize(btnWidth, btnHeight);
    m_localhost->setFixedSize(btnWidth, btnHeight);
    m_port->setFixedSize(btnWidth, btnHeight);
    m_backBtn->setFixedSize(btnWidth, btnHeight);

    m_startBtn->setFont(btnFont);
    m_stopBtn->setFont(btnFont);
    m_localhost->setFont(btnFont);
    m_port->setFont(btnFont);

    m_startBtn->setText("Запустить сервер");
    m_stopBtn->setText("Остановить сервер");
    m_backBtn->setText("Назад");

    m_localhost->setText(QString("IP: %1").arg(m_server->serverIp()));
    m_port->setText(QString("%1").arg(QString::number(m_server->serverPort())));

    m_grid->setMargin(10);
    m_grid->addWidget(m_localhost, 0, 0);
    m_grid->addWidget(m_port, 1, 0);
    m_grid->addWidget(m_startBtn, 0, 1);
    m_grid->addWidget(m_stopBtn, 1, 1);
    m_grid->addWidget(m_backBtn, 2, 1);

    this->setLayout(m_grid);
}

void ServerSettingsView::startServer()
{
    m_server->setPort(m_port->text().toInt());
    m_server->startServer();
}
