#ifndef SQLDATABASE_H
#define SQLDATABASE_H

#include "global.h"

#include <QObject>

class QSqlTableModel;
class QSqlDatabase;

class SqlDBSaver : public QObject
{
    Q_OBJECT
public:
    explicit SqlDBSaver(QObject *parent = 0);

public slots:
    void createTestTables(const QString &dbName);
    void createResultTable(const QString &dbName, const QString &tableName);
    void saveStudentResultToDb(const QString &dbName, const QString &tableName, const StudentResult &result);
    void saveTestToDb(const QString &dbName, const TestData &result);

private:
};

#endif // SQLDATABASE_H
