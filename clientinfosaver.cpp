#include "clientinfosaver.h"
#include "sqldatabase.h"

#include <QDataStream>

ClientInfoSaver::ClientInfoSaver(qintptr id, QObject *parent) :
    QThread(parent),
    m_db(new SqlDBSaver(this))
{
    qRegisterMetaType<StudentResult>("StudentResult");
    connect(this, &ClientInfoSaver::saveResultToDataBase, m_db, &SqlDBSaver::saveResultToDataBase);
    this->m_socketDescriptor = id;
}

void ClientInfoSaver::run()
{
    qDebug() << " Thread started";

    m_socket = new QTcpSocket();

    // set the ID
    if(!m_socket->setSocketDescriptor(this->m_socketDescriptor))
    {
        emit error(m_socket->error());
        return;
    }

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    // We'll have multiple clients, we want to know which is which
    qDebug() << m_socketDescriptor << " Client connected";

    exec();
}

void ClientInfoSaver::readyRead()
{
    if (m_socket) {
        QByteArray clientData = m_socket->readAll();
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

        emit saveResultToDataBase(clientParcedData);
    }
}

StudentResult ClientInfoSaver::fillResultStructure(const QStringList &dataList) const
{
    StudentResult result;
    if (dataList.count() >= 5) {
        result.firstName  = dataList.at(0);
        result.secondName = dataList.at(1);
        result.surname    = dataList.at(2);
        result.group      = dataList.at(3);
        result.score      = dataList.at(4).toInt();
    }

    return result;
}

void ClientInfoSaver::disconnected()
{
    qDebug() << m_socketDescriptor << " Disconnected";

    m_socket->deleteLater();
    exit(0);
}
