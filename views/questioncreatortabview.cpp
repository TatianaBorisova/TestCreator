#include "questioncreatortabview.h"
#include "subviews/questioneditorsubview.h"
#include "subviews/testeditorsubview.h"
#include "sqldatabase.h"

#include <QDebug>

QuestionCreatorTabView::QuestionCreatorTabView(QWidget *parent) :
    TestCreatorBaseView(parent),
    m_questionEditor(new QuestionEditorSubView(this)),
    m_testData(new TestEditorSubView(this)),
    m_dbSave(new SqlDBSaver(this))
{
    showSubView(TestEditor);
    connect(m_testData, SIGNAL(showSubView(SubViews)), this, SLOT(showSubView(SubViews)));
    connect(m_questionEditor, SIGNAL(showSubView(SubViews)), this, SLOT(showSubView(SubViews)));
    connect(m_testData, &TestEditorSubView::testNameChanged, this, &QuestionCreatorTabView::testNameChanged);
    connect(m_testData, &TestEditorSubView::testTimeChanged, this, &QuestionCreatorTabView::testTimeChanged);
    connect(m_testData, &TestEditorSubView::questionCountChanged, this, &QuestionCreatorTabView::questionCountChanged);
}

void QuestionCreatorTabView::resize()
{
    m_questionEditor->setFixedSize(width(), height());
    m_testData->setFixedSize(width(), height());
}

void QuestionCreatorTabView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    resize();
}

void QuestionCreatorTabView::showSubView(SubViews view)
{
    m_questionEditor->hide();
    m_testData->hide();

    switch(view) {
    case QuestionEditor:
        m_questionEditor->show();
        break;
    case TestEditor:
    default:
        m_testData->show();
        break;
    }
}

void QuestionCreatorTabView::testDbChanged(const QString &dbname)
{
    qDebug() << "cnahged to " << dbname;
    m_dbName = dbname;
}

void QuestionCreatorTabView::testNameChanged(const QString &name)
{
    qDebug() << "cnahged to " << name;
    m_Data.testName = name;
}

void QuestionCreatorTabView::testTimeChanged(const QTime &time)
{
    qDebug() << "cnahged to " << time;
    m_Data.testTime = time;
}

void QuestionCreatorTabView::questionCountChanged(int value)
{
    qDebug() << "cnahged to " << value;
    m_Data.questionCount = value;
}

void QuestionCreatorTabView::saveTestToDB()
{
    m_dbSave->createTestTables(m_dbName);
    m_dbSave->saveTestToDb(m_dbName, m_Data);
}
