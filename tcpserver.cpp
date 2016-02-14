#include "tcpserver.h"
#include "global.h"

#include <QTcpServer>
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
    QObject(parent),
    m_tcpServer(new QTcpServer(this))
{
    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(newUser()));
    startServer();
}

void TcpServer::startServer()
{
    if (!m_tcpServer->listen(QHostAddress::Any, port) && m_serverStatus == 0) {
        QMessageBox::warning(0, "Error", QString("Unable to start the server: %1.").arg(m_tcpServer->errorString()));
    } else {
        m_serverStatus = 1;
        qDebug() << QString::fromUtf8("Сервер запущен!");
    }
}

void TcpServer::stopServer()
{
    if (m_serverStatus == 1) {
        foreach (int i, m_clients.keys()) {

            QTextStream os(m_clients[i]);
            os.setAutoDetectUnicode(true);
            os << QDateTime::currentDateTime().toString() << "\n";

            m_clients[i]->close();
            m_clients.remove(i);
        }

        m_tcpServer->close();
        QMessageBox::warning(0, "Error", "Сервер остановлен!");
        m_serverStatus = 0;
    }
}

void TcpServer::newUser()
{
    if (m_serverStatus == 1) {
        qDebug() << QString::fromUtf8("У нас новое соединение!");

        QTcpSocket* clientSocket = m_tcpServer->nextPendingConnection();
        int idusersocs = clientSocket->socketDescriptor();

        m_clients[idusersocs] = clientSocket;
        connect(m_clients[idusersocs], SIGNAL(readyRead()), this, SLOT(readClientSlot()));
    }
}

void TcpServer::closeClientConnection()
{
    QTcpSocket* clientSocket = dynamic_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        int idusersocs = clientSocket->socketDescriptor();

        clientSocket->close();
        m_clients.remove(idusersocs);
    }
}

void TcpServer::readClientSlot()
{
    QTcpSocket* clientSocket = dynamic_cast<QTcpSocket*>(sender());

    if (clientSocket) {
        QByteArray clientData = clientSocket->readAll();
        QDataStream clientDataStream(&clientData, QIODevice::ReadOnly);

        //read header mag
        char *headerBuffer = new char[headerMsgSize];
        clientDataStream.readRawData(headerBuffer, headerMsgSize);

        //convert to int
        QString msgSize = QString(headerBuffer);
        int msgSizeNumber = msgSize.toInt();

        //read using sum of data
        char *dataBuffer = new char[msgSizeNumber - headerMsgSize];
        clientDataStream.readRawData(dataBuffer, msgSizeNumber - headerMsgSize);

        QString fullMsg = QString(dataBuffer);
        QStringList dataList = fullMsg.split(";");

        StudentResult clientParcedData = fillResultStructure(dataList);
        qDebug() << clientParcedData.firstName
                 << clientParcedData.secondName
                 << clientParcedData.surname
                 << clientParcedData.group
                 << clientParcedData.score;

        //closeClientConnection();
    }
}

StudentResult TcpServer::fillResultStructure(const QStringList &dataList) const
{
    StudentResult result;
    if (dataList.count() > 0) {
        result.firstName  = dataList.at(0);
        result.secondName = dataList.at(1);
        result.surname    = dataList.at(2);
        result.group      = dataList.at(3);
        result.score      = dataList.at(4).toInt();
    }

    return result;
}

