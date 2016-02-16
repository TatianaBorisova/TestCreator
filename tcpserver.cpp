#include "tcpserver.h"
#include "global.h"
#include "clientinfosaver.h"

#include <QTcpSocket>
#include <QMessageBox>
#include <QTextStream>
#include <QMap>
#include <QDateTime>
#include <QDebug>

namespace {
const int port = 33333;
}

TcpServer::TcpServer(QObject *parent) :
    QTcpServer(parent),
    m_listeningPort(port)
{
    // startServer();
}

void TcpServer::startServer()
{
    if (!this->listen(QHostAddress::Any, m_listeningPort)) {
        QMessageBox::warning(0, "Error", QString("Unable to start the server: %1.").arg(this->errorString()));
    } else {
        qDebug() << QString::fromUtf8("Сервер запущен!");
    }
}

void TcpServer::setPort(int portvalue)
{
    m_listeningPort = portvalue;
}

QString TcpServer::serverIp() const
{
    return m_listeningIp;
}

int TcpServer::serverPort() const
{
    return m_listeningPort;
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    // We have a new connection
    qDebug() << socketDescriptor << " Connecting...";

    ClientInfoSaver *thread = new ClientInfoSaver(socketDescriptor, this);

    if (m_clients.count() > 0) {
        m_clients.insert(m_clients.count(), thread);
    } else {
        m_clients.insert(0, thread);
    }

    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(this, SIGNAL(closeClientConnection()), thread, SLOT(disconnected()));

    thread->start();
}

void TcpServer::stopServer()
{
    emit closeClientConnection();

    this->close();
    QMessageBox::warning(0, "Error", "Сервер остановлен!");
}
