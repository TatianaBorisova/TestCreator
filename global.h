#ifndef GLOBAL
#define GLOBAL

#include <QString>
#include <QList>
#include <QTime>

const int headerMsgSize = 16;

typedef struct answers {
    QString correctAnswer;
    QString uncorrectAnswers;
    QString imgPath;
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
    QString firstName;
    QString secondName;
    QString surname;
    QString group;
    int score;
    QList<AnswersVector> answerInfo;
} StudentResult;

typedef struct questions {
    QString question;
    int     weight;
    Answers answers;
} TestQuestions;

typedef struct testSt {
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
    QuestionViewer
};

#endif // GLOBAL

