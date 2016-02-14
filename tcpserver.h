#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "global.h"

#include <QObject>
#include <QMap>

class QTcpServer;
class QTcpSocket;
class QStringList;

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);

public slots:
    void startServer();
    void stopServer();
    void newUser();
    void readClientSlot();
    void closeClientConnection();

private:
    StudentResult fillResultStructure(const QStringList &dataList) const;

    QTcpServer            *m_tcpServer;
    int                    m_serverStatus;
    QMap<int,QTcpSocket *> m_clients;
};

#endif // TCPSERVER_H
