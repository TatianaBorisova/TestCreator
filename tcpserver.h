#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "global.h"

#include <QTcpServer>
#include <QMap>

class QTcpSocket;
class QStringList;
class ClientInfoSaver;

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);

    void setPort(int portvalue);
    QString serverIp() const;
    int serverPort() const;

public slots:
    void startServer();
    void stopServer();
    void setTestFolderPath(const QString &path);
    void sendTestFolder();

signals:
    void serverStarted();
    void closeClientConnection();
    void resultDbName(const QString &name);
    void testFolderRequest(const QString &path);

protected:
    void incomingConnection(qintptr socketDescriptor);

private slots:
    void saveResultDbName(const QString &name);

private:
    QMap<int, ClientInfoSaver*> m_clients;
    int                         m_listeningPort;
    QString                     m_listeningIp;
    QString                     m_resultDb;
    QString                     m_testsFolderPath;
};

#endif // TCPSERVER_H
