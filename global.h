#ifndef GLOBAL
#define GLOBAL

#include <QString>
#include <QList>
#include <QTime>

//block consts for TCP messaging
const int headerMsgSize = 16;
const QString cmdMsg = QString("GETFILES");
const QString downloadMsg = QString("DOWNLOADFILE");
const QString newfileMsg = QString("NEWFILENAME");
const QString newentryMsg = QString("NEWFILEENTRY");
const int cmdSize = 8;

const QString testTypeStr = QString("Утверждения");

typedef struct answers {
    QString correctAnswer;
    QString uncorrectAnswers;
    QString imgName;
    QByteArray image;
} Answers;

typedef struct test{
    QString question;
    QString trueAnswer;
    QString falseAnswer;
} TestStructure ;

typedef struct answersinfo {
    int     id;
    QString statement;
    QString chosenAnswer;
    int isCorrectAnswer;
    int assurance;
} AnswersVector;

typedef struct student {
    int id;
    QString testName;
    QString firstName;
    QString secondName;
    QString surname;
    QString group;
    int score;
    int maxPosibleScore;
    QString time;
    QList<AnswersVector> answerInfo;
} StudentResult;

typedef struct questions {
    QString question;
    int     weight;
    Answers answers;
} TestQuestions;

enum TestType {
    NoTypeTest = -1,
    StatementTest = 0,
    QuestionTest
};

typedef struct testSt {
    int id;
    QString testName;
    TestType testType;
    QTime testTime;
    int questionCount;
    QList<TestQuestions> questions;
} TestData;

enum TestViews {
    StartView = 0,
    CreatorView = StartView,
    SettingsView,
    ResultsView
};

enum SubViews {
    TestEditor = 0,
    QuestionEditor,
    QuestionViewer,
    DbViewer
};

typedef struct testHeader {
    int id;
    QString  testName;
    TestType testType;
    QTime    testTime;
    int      questionCount;
} TestHeaderData;

#endif // GLOBAL

