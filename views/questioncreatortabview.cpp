#include "questioncreatortabview.h"
#include "subviews/questioneditorsubview.h"
#include "subviews/testeditorsubview.h"
#include "sqldatabase.h"
#include "docfileprocessing.h"

#include <QDebug>
#include <QMessageBox>

QuestionCreatorTabView::QuestionCreatorTabView(QWidget *parent) :
    TestCreatorBaseView(parent),
    m_questionEditor(new QuestionEditorSubView(this)),
    m_testData(new TestEditorSubView(this)),
    m_dbSave(new SqlDBSaver(this)),
    m_docReader(new DocFileProcessing(this))
{
    showSubView(TestEditor);
    connect(m_testData, SIGNAL(showSubView(SubViews)), this, SLOT(showSubView(SubViews)));
    connect(m_questionEditor, SIGNAL(showSubView(SubViews)), this, SLOT(showSubView(SubViews)));
    connect(m_questionEditor, &QuestionEditorSubView::createdQuestion, this, &QuestionCreatorTabView::insertQuestion);
    connect(m_testData, &TestEditorSubView::testNameChanged, this, &QuestionCreatorTabView::testNameChanged);
    connect(m_questionEditor, &QuestionEditorSubView::questionCounterChanged, this, &QuestionCreatorTabView::updatedValue);
    connect(m_testData, &TestEditorSubView::testTimeChanged, this, &QuestionCreatorTabView::testTimeChanged);
    connect(m_testData, &TestEditorSubView::questionCountChanged, this, &QuestionCreatorTabView::questionCountChanged);
    connect(this, &QuestionCreatorTabView::updatedIndexData, m_questionEditor, &QuestionEditorSubView::updatedIndexData);
    connect(m_testData, &TestEditorSubView::saveDataInDb, this, &QuestionCreatorTabView::saveDataInDb);
    connect(m_testData, &TestEditorSubView::loadedDocFile, this, &QuestionCreatorTabView::loadDataFromDocFile);
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

void QuestionCreatorTabView::updatedValue(int index)
{
    if (m_data.questions.count() > index) {
        emit updatedIndexData(m_data.questions.at(index));
    }
}

void QuestionCreatorTabView::insertQuestion(const TestQuestions &test, int index)
{
    if (m_data.questions.count() > index) {
        m_data.questions.takeAt(index);
        m_data.questions.insert(index, test);
    }
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
    qDebug() << "changed to " << dbname;
    m_dbName = dbname;
}

void QuestionCreatorTabView::testNameChanged(const QString &name)
{
    qDebug() << "changed to " << name;
    m_data.testName = name;
}

void QuestionCreatorTabView::testTimeChanged(const QTime &time)
{
    qDebug() << "changed to " << time;
    m_data.testTime = time;
}

void QuestionCreatorTabView::questionCountChanged(int value)
{
    qDebug() << "changed to " << value;
    m_data.questionCount = value;
    m_data.questions.clear();

    //fill empty values
    for (int i = 0; i < m_data.questionCount; i++) {
        m_data.questions.append(TestQuestions());
    }

    //set question counter in question UI
    m_questionEditor->setQuestionsCount(m_data.questionCount);
}

void QuestionCreatorTabView::saveDataInDb()
{
    if (m_data.questionCount <= 0
            || m_data.testName.isEmpty()
            || m_data.testTime.isNull()) {
        QMessageBox::warning(0, "error", "Пожалуйста, сперва заполните данные о тесте");
        return;
    }

    for (int i = 0; i < m_data.questions.count(); i++) {
        if (m_data.questions.at(i).question.isEmpty()
                || m_data.questions.at(i).answers.correctAnswer.isEmpty()
                || m_data.questions.at(i).answers.uncorrectAnswers.isEmpty()) {
            QMessageBox::warning(0, "error", QString("Пожалуйста, сперва заполните вопросы.\n Вопрос %1 не заполнен.").arg(QString::number(i + 1)));
            return;
        }
    }

    m_dbSave->createTestTables(m_dbName);
    m_dbSave->saveTestToDb(m_dbName, m_data);
}

void QuestionCreatorTabView::loadDataFromDocFile(const QString &name)
{
    if (!name.isEmpty()) {
        m_data = m_docReader->readFromDocFile(name, this);
    }
}

void QuestionCreatorTabView::printTestData()
{
    qDebug() << "question count = " << m_data.questions.count() << "entered count = " << m_data.questionCount;

    qDebug() << "testName = " << m_data.testName;
    qDebug() << "testTime = " << m_data.testTime;
    qDebug() << "questionCount = " << m_data.questionCount;
    for (int i = 0; i < m_data.questions.count(); i++) {
        qDebug() << "question = " << m_data.questions.at(i).question;
        qDebug() << "weight = " << m_data.questions.at(i).weight;
        qDebug() << "correctAnswer = " << m_data.questions.at(i).answers.correctAnswer;
        qDebug() << "uncorrectAnswers = " << m_data.questions.at(i).answers.uncorrectAnswers;
        qDebug() << "imgPath = " << m_data.questions.at(i).answers.imgPath;
    }
}
