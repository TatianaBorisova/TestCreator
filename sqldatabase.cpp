#include "sqldatabase.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>

#include <QDateTime>

#include <QTableView>
#include <QMessageBox>
#include <QString>
#include <QDebug>

QString SqlDBSaver::m_dbName = "studentsbase";
QString SqlDBSaver::m_tableName = "students";

SqlDBSaver::SqlDBSaver(QObject *parent) :
    QObject(parent)
{
    createTableInDataBase(m_dbName, m_tableName);
}

void SqlDBSaver::saveResultToDataBase(const StudentResult &result)
{
    QSqlDatabase dbPtr = QSqlDatabase::addDatabase("QSQLITE");
    dbPtr.setDatabaseName(m_dbName);
    if (!dbPtr.open()) {
        QMessageBox::critical(0, "Can not open database", "Не могу открыть базу данных.\n");
        return;
    }

    // populate with some data
    QSqlQuery q_insert(dbPtr);
    q_insert.prepare("INSERT INTO "
                     + m_tableName
                     + " (firstname, secondName, surname, groupname, scorevalue, testdate)"
                     + " VALUES ( :firstname, :secondName, :surname, :groupname, :scorevalue, :testdate)");

    q_insert.bindValue(":firstname", result.firstName);
    q_insert.bindValue(":secondName", result.secondName);
    q_insert.bindValue(":surname", result.surname);
    q_insert.bindValue(":groupname", result.group);
    q_insert.bindValue(":scorevalue", result.score);
    q_insert.bindValue(":testdate", QDateTime::currentDateTime());

    qDebug() << "insert data row: " << q_insert.exec();
}

void SqlDBSaver::createTableInDataBase(const QString &dbName, const QString &tableName)
{

    QSqlDatabase dbPtr = QSqlDatabase::addDatabase("QSQLITE");
    dbPtr.setDatabaseName(dbName);
    if (!dbPtr.open()) {
        QMessageBox::critical(0, "Can not open database", "Не могу открыть " + dbName + " базу данных.\n");
        return;
    }

    // create a table in the memory DB
    QSqlQuery q_create = dbPtr.exec("CREATE TABLE "
                                 + tableName
                                 + " (id integer primary key autoincrement, firstname varchar(155), secondName varchar(155), surname varchar(155), groupname varchar(155), scorevalue int, testdate DATETIME)");
    qDebug() << "create: " << q_create.lastError();

    dbPtr.close();
}
