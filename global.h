#ifndef GLOBAL
#define GLOBAL

#include <QString>
#include <QList>
#include <QTime>

const int headerMsgSize = 16;

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
    QString statement;
    bool isCorrectAnswer;
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

typedef struct testSt {
    int id;
    QString testName;
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
    QString testName;
    QTime testTime;
    int questionCount;
} TestHeaderData;

#endif // GLOBAL

