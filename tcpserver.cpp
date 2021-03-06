#include "tcpserver.h"
#include "global.h"
#include "clientdatamanager.h"

#include <QTcpSocket>
#include <QMessageBox>
#include <QTextStream>
#include <QMap>
#include <QDateTime>
#include <QDebug>

#include <QHostInfo>
#include <QNetworkInterface>

namespace {
const int port = 33333;
}

TcpServer::TcpServer(QObject *parent) :
    QTcpServer(parent),
    m_listeningPort(port)
{
    connect(this, &TcpServer::resultDbName, this, &TcpServer::saveResultDbName);
}

void TcpServer::startServer()
{
    if (this->isListening()) {
        QMessageBox::warning(0, "Внимание", QString("Сервер уже запущен."));
        return;
    }

    if (!this->listen(QHostAddress::AnyIPv4, m_listeningPort)) {
        QMessageBox::warning(0, "Ошибка", QString("Не могу запустить сервер. Системная ошибка: %1.").arg(this->errorString()));
    } else {
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

        // use the first non-localhost IPv4 address
        for (int i = 0; i < ipAddressesList.size(); ++i) {
            if (!ipAddressesList.at(i).isNull()
                    && ipAddressesList.at(i) != QHostAddress::LocalHost
                    && ipAddressesList.at(i).protocol() == QAbstractSocket::IPv4Protocol) {
                m_listeningIp = ipAddressesList.at(i).toString();
                qDebug() << "host = "<< this->serverIp() << this->serverPort();
                break;
            }
        }
        emit serverStarted();
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

    ClientDataManager *thread = new ClientDataManager(socketDescriptor);
    thread->saveDbName(m_resultDb);

    if (m_clients.count() > 0) {
        m_clients.insert(m_clients.count(), thread);
    } else {
        m_clients.insert(0, thread);
    }

    connect(thread, SIGNAL(testFolderRequest()), this, SLOT(sendTestFolder()), Qt::QueuedConnection);
    connect(this, SIGNAL(testFolderRequest(QString)), thread, SLOT(processTestFolder(QString)), Qt::QueuedConnection);
    connect(this, &TcpServer::resultDbName, thread, &ClientDataManager::saveDbName, Qt::QueuedConnection);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(this, SIGNAL(closeClientConnection()), thread, SLOT(disconnected()));

    thread->start();
}

void TcpServer::sendTestFolder()
{
    emit testFolderRequest(m_testsFolderPath);
}

void TcpServer::stopServer()
{
    emit closeClientConnection();
    this->close();
}

void TcpServer::saveResultDbName(const QString &name)
{
    m_resultDb = name;
}

void TcpServer::setTestFolderPath(const QString &path)
{
    qDebug() << "setTestFolderPath" << path;
    m_testsFolderPath = path;
}
