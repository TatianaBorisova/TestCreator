#ifndef QUESTIONCREATORTABVIEW_H
#define QUESTIONCREATORTABVIEW_H

#include "testcreatorbaseview.h"

class QuestionEditorSubView;
class TestEditorSubView;
class SqlDBSaver;

class QuestionCreatorTabView : public TestCreatorBaseView
{
    Q_OBJECT

public:
    explicit QuestionCreatorTabView(QWidget *parent = 0);

    void setFixedSize(int w, int h);

public slots:
    void testDbChanged(const QString &dbname);
    void testNameChanged(const QString &name);
    void testTimeChanged(const QTime &time);
    void questionCountChanged(int value);

protected slots:
    void showSubView(SubViews view);
    void saveTestToDB();

protected:
    virtual void resize();

private:
    QuestionEditorSubView *m_questionEditor;
    TestEditorSubView     *m_testData;
    //creating test data
    TestData               m_Data;
    SqlDBSaver            *m_dbSave;
    QString                m_dbName;
};

#endif // QUESTIONCREATORTABVIEW_H
