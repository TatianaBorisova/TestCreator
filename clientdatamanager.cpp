#include "clientdatamanager.h"
#include "sqldatabase.h"

#include <QDataStream>
#include <QDebug>
#include <QDir>

ClientDataManager::ClientDataManager(qintptr id, QObject *parent) :
    QThread(parent),
    m_db(new SqlliteDbManager(this))
{
    qRegisterMetaType<StudentResult>("StudentResult");

    connect(this, &ClientDataManager::saveResultToDataBase, m_db, &SqlliteDbManager::saveStudentResultToDb);
    this->m_socketDescriptor = id;
}

void ClientDataManager::run()
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

void ClientDataManager::readyRead()
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

            qDebug() << "size = " << (msgSize - headerMsgSize) << "fullMsg = " << fullMsg;
            //check if it is cmd msg
            if ((msgSize - headerMsgSize) == cmdSize && fullMsg.contains(cmdMsg)) {
                processRequestCmdMsg(fullMsg);
            } else if (fullMsg.contains(downloadMsg)) {
                processRequestDownloadFileMsg(fullMsg);
            } else {
                saveTestResultInDbMsg(fullMsg);
            }
        }
    }
}

void ClientDataManager::saveTestResultInDbMsg(const QString &str)
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

    qDebug() << "answers count = " << clientParcedData.answerInfo.count();
    for (int i = 0; i < clientParcedData.answerInfo.count(); i ++) {
        qDebug() << clientParcedData.answerInfo.at(i).assurance
                 << clientParcedData.answerInfo.at(i).chosenAnswer
                 << clientParcedData.answerInfo.at(i).isCorrectAnswer
                 << clientParcedData.answerInfo.at(i).statement;
    }

    emit saveResultToDataBase(m_resultDbName, clientParcedData);
}

void ClientDataManager::processRequestCmdMsg(const QString &str)
{
    qDebug() << "requestCmdMsg" << str;
    emit testFolderRequest();
}

void ClientDataManager::processRequestDownloadFileMsg(const QString &str)
{
    qDebug() << "processRequestDownloadFileMsg" << str;
    QString filename = str;

    filename = filename.remove(downloadMsg);
    if (!filename.isEmpty()) {

        qDebug() << "real file name " << filename;

        QFile file(filename);
        if (file.exists()) {
            QByteArray bytes;
            if (file.open(QIODevice::ReadOnly))
                bytes = file.readAll();

            qDebug() << "FILE SIZE = " << file.size();
            qDebug() << "bytes SIZE = " << bytes.size();

            int lastSlashIndx = filename.lastIndexOf("/");
            int lastPointIndx = filename.lastIndexOf(".");

            sendFileToClient(bytes, filename.mid(lastSlashIndx + 1, lastPointIndx - (lastSlashIndx + 1)));

            file.close();
        }
    }
}

StudentResult ClientDataManager::fillResultStructure(const QStringList &dataList) const
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

        for (int i = 7; i < dataList.count(); i += 4) {
            AnswersVector vector;
            vector.statement       = dataList.at(i);
            vector.chosenAnswer    = dataList.at(i + 1);
            vector.isCorrectAnswer = dataList.at(i + 2).toInt();
            vector.assurance       = dataList.at(i + 3).toInt();
            result.answerInfo.append(vector);
        }
    }

    return result;
}

void ClientDataManager::disconnected()
{
    qDebug() << m_socketDescriptor << " Disconnected";

    m_socket->deleteLater();
    exit(0);
}

void ClientDataManager::saveDbName(const QString &db)
{
    m_resultDbName = db;
}

void ClientDataManager::processTestFolder(const QString &testFolder)
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
                if (SqlliteDbManager::checkIfTestDb(filesList.at(i))) {
                    filesString += QString( folder + filesList.at(i) + ";;");
                }
            }
            sendFilelistToClient(filesString);
        }
    }
}

void ClientDataManager::sendFilelistToClient(const QString &data)
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

        qDebug() << "sendFilelistToClient arr = " << arrBlock;
        //send data to server
        qDebug() << m_socket->write(arrBlock);
        m_socket->flush();
        m_socket->waitForBytesWritten(3000);
    }
}

void ClientDataManager::sendFileToClient(const QByteArray &data, const QString &filename)
{
    QByteArray filenameArr = filename.toUtf8();

    //calculate msg sum
    int msgSize = headerMsgSize + newfileMsg.length() + filenameArr.length() + newentryMsg.length() + data.length();

    //put data to bytearray
    QByteArray  arrBlock;
    arrBlock.fill(0, msgSize);
    arrBlock.insert(0, QString::number(msgSize));
    arrBlock.insert(headerMsgSize, newfileMsg);
    arrBlock.insert(headerMsgSize + newfileMsg.length(), filenameArr);
    arrBlock.insert(headerMsgSize + newfileMsg.length() + filenameArr.length(), newentryMsg);
    arrBlock.insert(headerMsgSize + newfileMsg.length() + filenameArr.length() + newentryMsg.length(), data);
    arrBlock.resize(msgSize);

    qDebug() << "sendFileToClient arr = " << arrBlock;
    //send data to server
    qDebug() << m_socket->write(arrBlock);
    m_socket->flush();
    m_socket->waitForBytesWritten(3000);
}
