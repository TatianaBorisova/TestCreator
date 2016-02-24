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
    void readFromDocSet(const QString &filename, QWidget *parent);

    QString generateTestFile() const;
    void clearData();
    void printReadData();

private:
    QString getStatementString();
    QString getTestNameString(const QString &filetext);
    QString getTestTimeString(const QString &filetext);
    QString getQuestCountString(const QString &filetext);
    QString firstTestString(int firstIndex, QString readStatement);
    QString clearAnswerString(const QString &str);
    QString clearStatementString(const QString &str);
    void fillTestQuestionInfo(QString str);
    void setSavingFileName(const QString &file);
    QString getFileName(const QString &file, bool withFileExtention = false) const;

    QStringList     m_testList;
    QStringList     m_statementList;
    QList <Answers> m_answerList;
    QString         m_docTextString;
    QAxWidget      *m_wordApp;
    QString         m_testFileName;
    TestData        m_loadedData;
};

#endif // DOCFILEPROCESSING_H
