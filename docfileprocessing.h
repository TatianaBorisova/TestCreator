#ifndef DOCFILEPROCESSING_H
#define DOCFILEPROCESSING_H

#include "global.h"

#include <QObject>
#include <QAxWidget>
#include <QAxObject>
#include <QListWidget>

class DocFileProcessing : public QObject
{
    Q_OBJECT
public:
    explicit DocFileProcessing(QObject *parent = 0);

    TestData readFromDocFile(const QString &filename, QWidget *parent);

private:
    void clearData();
    void printReadData();

    QString getStatementString();
    QString getTestNameString(const QString &filetext);
    QString getTestTimeString(const QString &filetext);
    QString getTestTypeString(const QString &filetext);
    QString getQuestCountString(const QString &filetext);

    QString firstTestString(int firstIndex, QString readStatement);
    QString clearStatementString(const QString &str);

    bool fillTestQuestionInfo(QString str);
    void takeTestHeaderInfo();

    QStringList     m_testList;
    QStringList     m_statementList;
    QList <Answers> m_answerList;
    QString         m_docTextString;
    QAxWidget      *m_wordApp;
    TestData        m_loadedData;
};

#endif // DOCFILEPROCESSING_H
