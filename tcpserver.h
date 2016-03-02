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

signals:
    void serverStarted();
    void closeClientConnection();

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    QMap<int, ClientInfoSaver*> m_clients;
    int                         m_listeningPort;
    QString                     m_listeningIp;
};

#endif // TCPSERVER_H
