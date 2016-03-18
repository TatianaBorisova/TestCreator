#ifndef CLIENTDATAMANAGER_H
#define CLIENTDATAMANAGER_H

#include "global.h"

#include <QThread>
#include <QTcpSocket>

class SqlliteDbManager;

class ClientDataManager : public QThread
{
    Q_OBJECT
public:
    explicit ClientDataManager(qintptr id, QObject *parent = 0);
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
    void sendFilelistToClient(const QString &data);
    void sendFileToClient(const QByteArray &data, const QString &filename);

private:
    StudentResult fillResultStructure(const QStringList &dataList) const;
    void saveTestResultInDbMsg(const QString &str);
    void processRequestCmdMsg(const QString &str);
    void processRequestDownloadFileMsg(const QString &str);

private:
    QTcpSocket       *m_socket;
    qintptr           m_socketDescriptor;
    QString           m_resultDbName;
    SqlliteDbManager *m_db;
};

#endif // CLIENTDATAMANAGER_H
