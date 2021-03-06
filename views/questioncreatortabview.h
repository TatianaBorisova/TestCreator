#ifndef QUESTIONCREATORTABVIEW_H
#define QUESTIONCREATORTABVIEW_H

#include "testcreatorbaseview.h"

class QuestionEditorSubView;
class TestEditorSubView;
class SqlliteDbManager;
class DocFileProcessing;
class QuestionsListSubView;
class DbTestViewer;

class QuestionCreatorTabView : public TestCreatorBaseView
{
    Q_OBJECT

public:
    explicit QuestionCreatorTabView(QWidget *parent = 0);

    void setFixedSize(int w, int h);

signals:
    void updatedIndexData(const TestQuestions &test);
    void loadTestData(const TestData &data);

public slots:
    void testDbChanged(const QString &dbname);
    void testNameChanged(const QString &name);
    void testTimeChanged(const QTime &time);
    void testTypeChanged(int type);
    void questionCountChanged(int value);
    void insertQuestion(const TestQuestions &test, int index);
    void updatedValue(int index);
    void saveDataInDb();
    void loadDataFromDocFile(const QString &name);
    void loadDataFromDBFile(const TestData &data);
    void loadDbData(const QString &name);
    void setLoadedQuestionsCount(int value);

protected slots:
    void showSubView(SubViews view);
    void loadTestList(const QList<TestHeaderData> &list);

protected:
    virtual void resize();

private:
    void printTestData();
    void hidePreviuosWindows();

    QuestionEditorSubView *m_questionEditor;
    TestEditorSubView     *m_testData;
    QuestionsListSubView  *m_questionsViewer;
    //creating test data
    TestData               m_data;
    SqlliteDbManager      *m_dbSave;
    QString                m_dbName;
    DocFileProcessing     *m_docReader;
    DbTestViewer          *m_dbViewer;
};

#endif // QUESTIONCREATORTABVIEW_H
