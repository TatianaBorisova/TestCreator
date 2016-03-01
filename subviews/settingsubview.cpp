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
    m_testBox(new QPlainTextEdit(this)),
    m_resultBox(new QPlainTextEdit(this)),
    m_startBtn(new QPushButton(this)),
    m_stopBtn(new QPushButton(this)),
    m_chooseTestDb(new QPushButton(this)),
    m_chooseResDb(new QPushButton(this)),
    m_rowBox(new QHBoxLayout(this))
{
    setFixedSize(500, 300);

    //tbd resolve stylesheets problem
    //this->setStyleSheet("QPushButton { min-height: 50px; }");
    //this->setStyleSheet("QPlainTextEdit { min-height: 50px; }");
    //this->setStyleSheet("QLineEdit { min-height: 50px; }");

    connect(m_startBtn, &QPushButton::clicked, this, &SettingSubView::startServer);
    connect(m_stopBtn,  &QPushButton::clicked, m_server, &TcpServer::stopServer);
    connect(m_chooseTestDb,  &QPushButton::clicked, this, &SettingSubView::chooseTestDB);
    connect(m_chooseResDb,  &QPushButton::clicked, this, &SettingSubView::chooseResDB);

    m_startBtn->setFixedHeight(btnHeight);
    m_stopBtn->setFixedHeight(btnHeight);
    m_localhost->setFixedHeight(btnHeight);
    m_port->setFixedHeight(btnHeight);
    m_testBox->setFixedHeight(btnHeight);
    m_resultBox->setFixedHeight(btnHeight);
    m_chooseTestDb->setFixedHeight(btnHeight);
    m_chooseResDb->setFixedHeight(btnHeight);

    m_testBox->setReadOnly(true);
    m_resultBox->setReadOnly(true);

    m_startBtn->setText("Запустить сервер");
    m_stopBtn->setText("Остановить сервер");
    m_chooseTestDb->setText("Выбрать базу тестов");
    m_chooseResDb->setText("Выбрать базу результатов");

    m_localhost->setText(QString("IP: %1").arg(m_server->serverIp()));
    m_port->setText(QString("PORT: %1").arg(QString::number(m_server->serverPort())));

    m_testBox->setPlainText(QDir::current().absolutePath() + QString("/testDb"));
    m_resultBox->setPlainText(QDir::current().absolutePath() + QString("/resultDb"));

    m_rowBox->addWidget(m_localhost);
    m_rowBox->addWidget(m_port);

    m_grid->setMargin(10);
    m_grid->addLayout(m_rowBox, 0, 0);
    m_grid->addWidget(m_startBtn, 0, 1);
    m_grid->addWidget(m_stopBtn, 1, 1);

    m_grid->addWidget(m_testBox, 2, 0);
    m_grid->addWidget(m_resultBox, 3, 0);
    m_grid->addWidget(m_chooseTestDb, 2, 1);
    m_grid->addWidget(m_chooseResDb, 3, 1);

    this->setLayout(m_grid);

    emit testDbChanged(m_testBox->toPlainText());
    emit resultDbChanged(m_resultBox->toPlainText());
}

void SettingSubView::chooseTestDB()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Choose Test Document"), "");
    m_testBox->setPlainText(filePath);
    emit testDbChanged(filePath);
}

void SettingSubView::chooseResDB()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Choose Test Document"), "");
    m_resultBox->setPlainText(filePath);
    emit resultDbChanged(filePath);
}

QString SettingSubView::testDb() const
{
    m_testBox->toPlainText();
}

QString SettingSubView::resultDb() const
{
    m_resultBox->toPlainText();
}

void SettingSubView::startServer()
{
    m_server->setPort(m_port->text().toInt());
    m_server->startServer();
}

void SettingSubView::resize()
{
    emit testDbChanged(m_testBox->toPlainText());
    emit resultDbChanged(m_resultBox->toPlainText());
}

void SettingSubView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    resize();
}
