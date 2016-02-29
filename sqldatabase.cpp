#include "sqldatabase.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlTableModel>

#include <QDateTime>
#include <QBuffer>
#include <QTableView>
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include <QDir>

SqlDBSaver::SqlDBSaver(QObject *parent) :
    QObject(parent)
{
}

void SqlDBSaver::saveStudentResultToDb(const QString &dbName, const QString &tableName, const StudentResult &result)
{
    QSqlDatabase dbPtr = QSqlDatabase::addDatabase("QSQLITE");
    dbPtr.setDatabaseName(dbName);
    if (!dbPtr.open()) {
        QMessageBox::critical(0, "Can not open database", "Не могу открыть базу данных.\n");
        return;
    }

    // populate with some data
    QSqlQuery q_insert(dbPtr);
    q_insert.prepare("INSERT INTO "
                     + tableName
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

void SqlDBSaver::saveTestToDb(const QString &dbName, const TestData &result)
{
    QSqlDatabase dbPtr = QSqlDatabase::addDatabase("QSQLITE");
    dbPtr.setDatabaseName(dbName);
    if (!dbPtr.open()) {
        QMessageBox::critical(0, "Can not open database", "Не могу открыть базу данных.\n");
        return;
    }

    //firstly lets check if test name already exists and delete it and all data connected with it
    QSqlQuery q_existed(dbPtr);
    q_existed.prepare("SELECT id FROM testdata WHERE testname=:testname");
    q_existed.bindValue(":testname", result.testName);
    if (q_existed.exec()) {
        int idField = q_existed.record().indexOf("id");
        int testId = 0;

        if (q_existed.next()) {
            QMessageBox::warning(0, "Warning", QString("Внимание, в базе найден тест с именем %1.\nОн будет заменен на текущий.").arg(result.testName));

            testId = q_existed.value(idField).toInt();
            QSqlQuery q_deleteId(dbPtr);
            q_deleteId.prepare("DELETE FROM questionsdata WHERE testid=:testid");
            q_deleteId.bindValue(":testid", testId);
            q_deleteId.exec();

            QSqlQuery q_delete(dbPtr);
            q_delete.prepare("DELETE FROM testdata WHERE testname=:testname");
            q_delete.bindValue(":testname", result.testName);
            q_delete.exec();
        }
    }


    // insert new test data
    QSqlQuery q_insert(dbPtr);
    q_insert.prepare("INSERT INTO testdata (testname, testtime, questioncount) VALUES ( :testname, :testtime, :questioncount)");

    q_insert.bindValue(":testname", result.testName);
    q_insert.bindValue(":testtime", result.testTime);
    q_insert.bindValue(":questioncount", result.questionCount);

    if (q_insert.exec()) {

        //связать по ид из первой табл.
        //то есть табл: id/id testa/correct answer/incorrect answer/imgpath
        QSqlQuery q_select(dbPtr);
        q_select.prepare("SELECT id FROM testdata WHERE testname=:testname AND questioncount=:questioncount");
        q_select.bindValue(":testname", result.testName);
        q_select.bindValue(":questioncount", result.questionCount);
        q_select.exec();

        int fieldNo = q_select.record().indexOf("id");
        int testId = 0;
        while (q_select.next()) {
            testId = q_select.value(fieldNo).toInt();

            for (int i = 0; i < result.questions.count(); i++) {
                QSqlQuery q_ins(dbPtr);
                q_ins.prepare("INSERT INTO questionsdata (testid, question, testweight, correctanswer, uncorrectanswers, imgname, image) VALUES (:testid, :question, :testweight, :correctanswer, :uncorrectanswers, :imgname, :image)");
                q_ins.bindValue(":testid", testId);
                q_ins.bindValue(":question", result.questions.at(i).question);
                q_ins.bindValue(":testweight", result.questions.at(i).weight);
                q_ins.bindValue(":correctanswer", result.questions.at(i).answers.correctAnswer);
                q_ins.bindValue(":uncorrectanswers", result.questions.at(i).answers.uncorrectAnswers);

                QPixmap inPixmap(result.questions.at(i).answers.imgName);

                //get file extention
                int index = result.questions.at(i).answers.imgName.lastIndexOf(".");
                QString extention = result.questions.at(i).answers.imgName.mid(index + 1, result.questions.at(i).answers.imgName.count() - index - 1);
                extention = extention.toUpper();

                //get file name
                int separator = result.questions.at(i).answers.imgName.lastIndexOf("/");
                QString filename = result.questions.at(i).answers.imgName.mid(separator + 1, result.questions.at(i).answers.imgName.count() - separator - 1);

                //load image to data base :-)!!!
                QByteArray inByteArray;
                QBuffer inBuffer( &inByteArray );
                inBuffer.open(QIODevice::WriteOnly);
                inPixmap.save(&inBuffer, extention.toStdString().c_str());

                q_ins.bindValue(":imgname", filename);
                q_ins.bindValue(":image", inByteArray);
                q_ins.exec();
            }
        }
    }

    dbPtr.close();
}

void SqlDBSaver::createTestTables(const QString &dbName)
{

    QSqlDatabase dbPtr = QSqlDatabase::addDatabase("QSQLITE");

    if (!QFile::exists(dbName)) {

        dbPtr.setDatabaseName(dbName);
        if (!dbPtr.open()) {
            QMessageBox::critical(0, "Can not open database", "Не могу открыть " + dbName + " базу данных.\n");
            return;
        }

        // create a table in the memory DB
        QSqlQuery q_testcreate = dbPtr.exec("CREATE TABLE testdata (id integer primary key autoincrement, testname varchar(255), testtime TIME, questioncount int)");
        qDebug() << "create: " << q_testcreate.lastError();


        QSqlQuery q_questcreate = dbPtr.exec("CREATE TABLE questionsdata (id integer primary key autoincrement, testid int, question varchar(2048), testweight int, correctanswer varchar(255), uncorrectanswers varchar(255), imgname varchar(255), image BLOB)");
        qDebug() << "create: " << q_questcreate.lastError();

        dbPtr.close();
    }
}

void SqlDBSaver::createResultTable(const QString &dbName, const QString &tableName)
{

}

void SqlDBSaver::loadTestDataFromDbFile(const QString &testName)
{
    TestData data;
    QSqlDatabase dbPtr = QSqlDatabase::addDatabase("QSQLITE");

    dbPtr.setDatabaseName(m_db);
    if (!dbPtr.open()) {
        QMessageBox::critical(0, "Can not open database", "Не могу открыть базу данных.\n");
        return;
    }

    QSqlQuery q_existed(dbPtr);
    q_existed.prepare("SELECT * FROM testdata WHERE testname=:testname");
    q_existed.bindValue(":testname", testName);

    if (q_existed.exec()) {
        while (q_existed.next()) {
            data.id            = q_existed.value(q_existed.record().indexOf("id")).toInt();
            data.testName      = q_existed.value(q_existed.record().indexOf("testname")).toString();
            data.testTime      = q_existed.value(q_existed.record().indexOf("testtime")).toTime();
            data.questionCount = q_existed.value(q_existed.record().indexOf("questioncount")).toInt();
        }
    }

    QSqlQuery q_questions(dbPtr);
    q_questions.prepare("SELECT * FROM questionsdata WHERE testid=:testid");
    q_questions.bindValue(":testid", data.id);
    if (q_questions.exec()) {
        while (q_questions.next()) {
            TestQuestions question;
            question.question = q_questions.value(q_questions.record().indexOf("question")).toString();
            question.weight = q_questions.value(q_questions.record().indexOf("testweight")).toInt();
            question.answers.correctAnswer = q_questions.value(q_questions.record().indexOf("correctanswer")).toString();
            question.answers.uncorrectAnswers = q_questions.value(q_questions.record().indexOf("uncorrectanswers")).toString();
            question.answers.imgName = q_questions.value(q_questions.record().indexOf("imgname")).toString();
            question.answers.image = q_questions.value(q_questions.record().indexOf("image")).toByteArray();
            data.questions.append(question);
        }
    }
    dbPtr.close();

    emit sendFullTestData(data);
}

void SqlDBSaver::loadDbFile(const QString &filename)
{
    QSqlDatabase dbPtr = QSqlDatabase::addDatabase("QSQLITE");
    dbPtr.setDatabaseName(filename);
    if (!dbPtr.open()) {
        QMessageBox::critical(0, "Can not open database", "Не могу открыть базу данных.\n");
        return;
    }

    m_db = filename;
    QList<TestHeaderData> list;

    //firstly lets check if test name already exists and delete it and all data connected with it
    QSqlQuery q_existed(dbPtr);
    q_existed.prepare("SELECT * FROM testdata");

    if (q_existed.exec()) {
        while (q_existed.next()) {

            TestHeaderData testData;
            testData.id            = q_existed.value(q_existed.record().indexOf("id")).toInt();
            testData.testName      = q_existed.value(q_existed.record().indexOf("testname")).toString();
            testData.testTime      = q_existed.value(q_existed.record().indexOf("testtime")).toTime();
            testData.questionCount = q_existed.value(q_existed.record().indexOf("questioncount")).toInt();
            list.append(testData);
        }
    }

    emit readTests(list);
    dbPtr.close();
}
