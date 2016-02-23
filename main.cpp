#include "mainwindow.h"

//////
#include "sqldatabase.h"
#include <QTime>
#include <QFileDialog>
////

#include <QApplication>

void testDBSaving()
{
    SqlDBSaver db;
    TestData data;

    data.testName = "Мой тестовый тестище";
    data.testTime = QTime(0, 10);
    data.questionCount = 2;
    for ( int i = 0 ; i < 2; i++ ) {
        TestQuestions testtest;
        testtest.question = QString("Кто тут супер изобретатель тестов?");
        testtest.weight = 10;
        testtest.answers.correctAnswer = QString("Я");
        testtest.answers.uncorrectAnswers = QString("Не я");

        QString filePath = QFileDialog::getOpenFileName(0, "Choose Test Document", "");

        testtest.answers.imgPath = filePath;
        data.questions.append(testtest);
    }

    db.createTestTables("testdb");
    db.saveTestToDb("testdb", data);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mainWnd;

    mainWnd.setWindowTitle("Eco Test Creator");

    mainWnd.showMainView(StartView);

    return a.exec();
}
