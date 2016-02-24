#ifndef QUESTIONCREATORTABVIEW_H
#define QUESTIONCREATORTABVIEW_H

#include "testcreatorbaseview.h"

class QuestionEditorSubView;
class TestEditorSubView;
class SqlDBSaver;
class DocFileProcessing;

class QuestionCreatorTabView : public TestCreatorBaseView
{
    Q_OBJECT

public:
    explicit QuestionCreatorTabView(QWidget *parent = 0);

    void setFixedSize(int w, int h);

signals:
    void updatedIndexData(const TestQuestions &test);

public slots:
    void testDbChanged(const QString &dbname);
    void testNameChanged(const QString &name);
    void testTimeChanged(const QTime &time);
    void questionCountChanged(int value);
    void insertQuestion(const TestQuestions &test, int index);
    void updatedValue(int index);
    void saveDataInDb();
    void loadDataFromDocFile(const QString &name);

protected slots:
    void showSubView(SubViews view);

protected:
    virtual void resize();

private:
    void printTestData();

    QuestionEditorSubView *m_questionEditor;
    TestEditorSubView     *m_testData;
    //creating test data
    TestData               m_data;
    SqlDBSaver            *m_dbSave;
    QString                m_dbName;
    DocFileProcessing     *m_docReader;
};

#endif // QUESTIONCREATORTABVIEW_H
