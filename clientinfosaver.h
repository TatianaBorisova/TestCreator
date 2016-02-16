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
    void saveResultToDataBase(const StudentResult &result);

public slots:
    void readyRead();
    void disconnected();

private:
    StudentResult fillResultStructure(const QStringList &dataList) const;

private:
    QTcpSocket *m_socket;
    qintptr     m_socketDescriptor;
    SqlDBSaver *m_db;
};

#endif // CLIENTINFOSAVER_H
