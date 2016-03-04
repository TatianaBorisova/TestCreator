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

signals:
    void sendFullTestData(const TestData &data);
    void readTests(const QList<TestHeaderData> &list);

public slots:
    void createTestTables(const QString &dbName);
    void createResultTable(const QString &dbName);
    void saveStudentResultToDb(const QString &db, const StudentResult &result);
    void saveTestToDb(const QString &dbName, const TestData &result);

    void loadDbFile(const QString &filename);
    void loadTestDataFromDbFile(const QString &testName);

private:
    QString m_testsdb;
};

#endif // SQLDATABASE_H
