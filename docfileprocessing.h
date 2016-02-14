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

    void readFromDocFile(const QString &filename, QWidget *parent);
    void readFromDocSet(const QString &filename, QWidget *parent);
    QString generateTestFile() const;
    void clearData();
    void printReadData();

private:
    QString getFirstTestString();
    QString firstTestString(int firstIndex, QString readStatement);
    QString clearAnswerString(const QString &str);
    QString clearStatementString(const QString &str);
    void fillTestQuestionInfo(QString str);

    QStringList m_testList;
    QStringList m_statementList;
    QList <Answers> m_answerList;
    QString     m_docTextString;
    QAxWidget  *m_wordApp;
};

#endif // DOCFILEPROCESSING_H
