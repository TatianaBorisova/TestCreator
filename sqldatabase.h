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
    void createTableInDataBase(const QString &dbName, const QString &tableName);
    void saveResultToDataBase(const StudentResult &result);

private:
    static QString m_dbName;
    static QString m_tableName;
};

#endif // SQLDATABASE_H
