#include "clientinfosaver.h"
#include "sqldatabase.h"

#include <QDataStream>
#include <QDebug>
#include <QDir>

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

    m_socket = new QTcpSocket(this);

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
            if ((msgSize - headerMsgSize) == cmdSize && fullMsg.contains(cmdMsg)) {
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
    qDebug() << "requestCmdMsg" << str;
    emit testFolderRequest();
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

void ClientInfoSaver::processTestFolder(const QString &testFolder)
{
    qDebug() << "processTestFolder" << testFolder;

    QString folder = testFolder;

    if (!folder.isEmpty()) {

        if (folder.at(folder.count() - 1) != '/')
            folder = folder + '/';

        QDir entryDir(folder);
        if (entryDir.exists()) {

            QStringList filesList = entryDir.entryList(QDir::Files | QDir::NoDotAndDotDot);
            QString filesString("");

            for (int i = 0; i < filesList.count(); i++) {
                if (SqlDBSaver::checkIfTestDb(filesList.at(i))) {
                   filesString += QString( folder + filesList.at(i) + ";;");
                }
            }
            sendDataToClient(filesString);
        }
    }
}

void ClientInfoSaver::sendDataToClient(const QString &data)
{
    if (data.length() > 0) {
        QString cmd = data;
        QByteArray bytes(cmd.toStdString().c_str());
        //calculate msg sum
        int msgSize = headerMsgSize + bytes.length();

        //put data to bytearray
        QByteArray  arrBlock;
        arrBlock.fill(0, msgSize);
        arrBlock.insert(0, QString::number(msgSize));
        arrBlock.insert(headerMsgSize, cmd);
        arrBlock.resize(msgSize);

        qDebug() << "server arr = " << arrBlock;
        //send data to server
        qDebug() << m_socket->write(arrBlock);
        m_socket->flush();
        m_socket->waitForBytesWritten(3000);
    }
}
