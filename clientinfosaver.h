#ifndef CLIENTINFOSAVER_H
#define CLIENTINFOSAVER_H

#include "global.h"

#include <QThread>
#include <QTcpSocket>
#include <QDebug>

class SqlDBSaver;

class ClientInfoSaver : public QThread
{
    Q_OBJECT
public:
    explicit ClientInfoSaver(qintptr id, QObject *parent = 0);
    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);
    void saveResultToDataBase(const QString &db, const StudentResult &result);
    void testFolderRequest();

public slots:
    void readyRead();
    void disconnected();
    void saveDbName(const QString &db);
    void processTestFolder(const QString &testFolder);
    void sendDataToClient(const QString &data);

private:
    StudentResult fillResultStructure(const QStringList &dataList) const;
    void saveResultDbMsg(const QString &str);
    void requestCmdMsg(const QString &str);

private:
    QTcpSocket *m_socket;
    qintptr     m_socketDescriptor;
    SqlDBSaver *m_db;
    QString     m_resultDbName;
};

#endif // CLIENTINFOSAVER_H
