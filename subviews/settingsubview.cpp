#include "settingsubview.h"
#include "tcpserver.h"

#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QDir>

namespace {
const int btnWidth = 200;
const int btnHeight = 50;
}

SettingSubView::SettingSubView(QWidget *parent) :
    TestCreatorBaseView(parent),
    m_grid(new QGridLayout(this)),
    m_server(new TcpServer(this)),
    m_localhost(new QLabel(this)),
    m_port(new QLineEdit(this)),
    m_serverStatus(new QLabel(this)),
    m_testBox(new QLineEdit(this)),
    m_resultBox(new QLineEdit(this)),
    m_startBtn(new QPushButton(this)),
    m_stopBtn(new QPushButton(this)),
    m_chooseTestDb(new QPushButton(this)),
    m_chooseResDb(new QPushButton(this)),
    m_rowBox(new QHBoxLayout(this))
{
    setFixedSize(500, 300);

    connect(m_startBtn, &QPushButton::clicked, this, &SettingSubView::startServer);
    connect(m_stopBtn,  &QPushButton::clicked, m_server, &TcpServer::stopServer);

    connect(m_chooseTestDb, &QPushButton::clicked, this, &SettingSubView::chooseTestDB);
    connect(m_chooseResDb,  &QPushButton::clicked, this, &SettingSubView::chooseResDB);

    connect(m_testBox,   &QLineEdit::editingFinished, this, &SettingSubView::testDbChangedSlot);
    connect(m_resultBox, &QLineEdit::editingFinished, this, &SettingSubView::resultDbChangedSlot);

    connect(m_server, &TcpServer::serverStarted, this, &SettingSubView::setStartSetverState);
    connect(m_server, &TcpServer::closeClientConnection, this, &SettingSubView::setStopSetverState);
    connect(this, &SettingSubView::testFolderPathChanged, m_server, &TcpServer::setTestFolderPath);
    connect(this, &SettingSubView::resultDbChanged, m_server, &TcpServer::resultDbName);

    m_testBox->setFixedHeight(btnHeight);
    m_resultBox->setFixedHeight(btnHeight);
    m_port->setFixedWidth(btnWidth);

    m_startBtn->setText("Запустить сервер");
    m_stopBtn->setText("Остановить сервер");
    m_chooseTestDb->setText("Выбрать базу тестов");
    m_chooseResDb->setText("Выбрать базу результатов");

    m_localhost->setText(QString("IP: %1").arg(m_server->serverIp()));
    m_port->setText(QString("%1").arg(QString::number(m_server->serverPort())));

    m_testBox->setText(QDir::current().absolutePath() + QString("/testDb"));
    m_resultBox->setText(QDir::current().absolutePath() + QString("/resultDb"));

    m_rowBox->addWidget(m_localhost);
    m_rowBox->addWidget(m_port);

    m_grid->setMargin(10);
    m_grid->addLayout(m_rowBox, 0, 0);
    m_grid->addWidget(m_startBtn, 0, 1);
    m_grid->addWidget(m_stopBtn, 1, 1);
    m_grid->addWidget(m_serverStatus, 1, 0);

    m_grid->addWidget(m_testBox, 2, 0);
    m_grid->addWidget(m_resultBox, 3, 0);
    m_grid->addWidget(m_chooseTestDb, 2, 1);
    m_grid->addWidget(m_chooseResDb, 3, 1);

    m_grid->setMargin(30);
    m_grid->setSpacing(30);

    this->setLayout(m_grid);

    emit testDbChanged(m_testBox->text());
    emit resultDbChanged(m_resultBox->text());
    setStopSetverState();
}

void SettingSubView::chooseTestDB()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Choose Test Document"), "");
    m_testBox->setText(filePath);
    emit testDbChanged(filePath);
}

void SettingSubView::chooseResDB()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Choose Test Document"), "");
    m_resultBox->setText(filePath);
    emit resultDbChanged(filePath);
}

void SettingSubView::testDbChangedSlot()
{
    emit testDbChanged(m_testBox->text());
}

void SettingSubView::resultDbChangedSlot()
{
    emit resultDbChanged(m_resultBox->text());
}

void SettingSubView::setStartSetverState()
{
    m_serverStatus->setText("Сервер запущен");
    m_localhost->setText(QString("IP: %1").arg(m_server->serverIp()));
}

void SettingSubView::setStopSetverState()
{
    m_serverStatus->setText("Сервер остановлен");
}

QString SettingSubView::testDb() const
{
    m_testBox->text();
}

QString SettingSubView::resultDb() const
{
    m_resultBox->text();
}

void SettingSubView::startServer()
{
    m_server->setPort(m_port->text().toInt());
    m_server->startServer();
}

void SettingSubView::resize()
{
    emit testDbChanged(m_testBox->text());
    emit resultDbChanged(m_resultBox->text());
}

void SettingSubView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    resize();
}
