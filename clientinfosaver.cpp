#include "clientinfosaver.h"
#include "sqldatabase.h"

#include <QDataStream>
#include <QDebug>

ClientInfoSaver::ClientInfoSaver(qintptr id, QObject *parent) :
    QThread(parent),
    m_db(new SqlDBSaver(this))
{
    qRegisterMetaType<StudentResult>("StudentResult");

    connect(this, &ClientInfoSaver::saveResultToDataBase, m_db, &SqlDBSaver::saveStudentResultToDb);
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
    qDebug() << "Client ready to read" << m_socket->bytesAvailable() << "bytes.";
    while(m_socket->bytesAvailable())
    {
        if (m_socket->bytesAvailable() >= headerMsgSize) {

            QByteArray buffer = m_socket->read(headerMsgSize);
            QByteArray newarray;

            for (int i = 0; i < buffer.size(); i++) {
                if (buffer.at(i) == '\0')
                    continue;

                newarray.append(buffer.at(i));
            }

            int msgSize = QString(newarray.toStdString().c_str()).toInt();
            buffer.clear();
            newarray.clear();

            buffer = m_socket->read(msgSize);

            while(buffer.size() < msgSize - headerMsgSize)
            {
                m_socket->waitForReadyRead(3000);
                buffer.append(m_socket->read(msgSize - buffer.size()));
            }

            for (int i = 0; i < buffer.size(); i++) {
                if (buffer.at(i) == '\0')
                    continue;

                newarray.append(buffer.at(i));
            }

            QString fullMsg(newarray.toStdString().c_str());
            //check if it is cmd msg
            if (msgSize == cmdSize && fullMsg.contains(cmdMsg)) {
                requestCmdMsg(fullMsg);
            } else {
                saveResultDbMsg(fullMsg);
            }
        }
    }
}

void ClientInfoSaver::saveResultDbMsg(const QString &str)
{
    QStringList dataList = str.split(";");
    StudentResult clientParcedData = fillResultStructure(dataList);

    qDebug() << clientParcedData.testName
             << clientParcedData.firstName
             << clientParcedData.secondName
             << clientParcedData.surname
             << clientParcedData.group
             << clientParcedData.score
             << clientParcedData.maxPosibleScore;

    emit saveResultToDataBase(m_resultDbName, clientParcedData);
}

void ClientInfoSaver::requestCmdMsg(const QString &str)
{

}

StudentResult ClientInfoSaver::fillResultStructure(const QStringList &dataList) const
{
    StudentResult result;
    qDebug() << "count = " << dataList.count();
    if (dataList.count() >= 7) {
        result.testName        = dataList.at(0);
        result.firstName       = dataList.at(1);
        result.secondName      = dataList.at(2);
        result.surname         = dataList.at(3);
        result.group           = dataList.at(4);
        result.score           = dataList.at(5).toInt();
        result.maxPosibleScore = dataList.at(6).toInt();
    }

    return result;
}

void ClientInfoSaver::disconnected()
{
    qDebug() << m_socketDescriptor << " Disconnected";

    m_socket->deleteLater();
    exit(0);
}

void ClientInfoSaver::saveDbName(const QString &db)
{
    m_resultDbName = db;
}
