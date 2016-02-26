#include "testeditorsubview.h"

#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QRegExpValidator>
#include <QTimeEdit>
#include <QMessageBox>
#include <QFileDialog>

namespace {
const int elHeight = 50;
const int elWidth = 300;
}

TestEditorSubView::TestEditorSubView(QWidget *parent) :
    TestCreatorBaseView(parent),
    m_box(new QGridLayout(this)),
    m_testName(new QLabel("Название теста", this)),
    m_testTime(new QLabel("Время выполнения теста", this)),
    m_questionCount(new QLabel("Количество вопросов", this)),
    m_testNameBox(new QLineEdit(this)),
    m_testTimeBox(new QTimeEdit(this)),
    m_questionCountBox(new QLineEdit(this)),
    m_addQuestions(new QPushButton("Редактировать вопросы", this)),
    m_loadFromDoc(new QPushButton("Загрузить тест из Doc файла", this)),
    m_saveinDb(new QPushButton("Сохранить в БД", this)),
    m_questionView(new QPushButton("Просмотреть вопросы", this))
{
    connect(m_addQuestions, &QPushButton::clicked, this, &TestEditorSubView::addQuestions);
    connect(m_testNameBox, &QLineEdit::editingFinished, this, &TestEditorSubView::setTestName);
    connect(m_testTimeBox, &QTimeEdit::editingFinished, this, &TestEditorSubView::setTestTime);
    connect(m_questionCountBox, &QLineEdit::editingFinished, this, &TestEditorSubView::setQuestionCount);
    connect(m_saveinDb, &QPushButton::clicked, this, &TestEditorSubView::saveDataInDb);
    connect(m_loadFromDoc, &QPushButton::clicked, this, &TestEditorSubView::loadDocFile);
    connect(m_questionView, &QPushButton::clicked, this, &TestEditorSubView::showQuestionsListView);

    m_testName->setFixedHeight(elHeight);
    m_testTime->setFixedHeight(elHeight);
    m_questionCount->setFixedHeight(elHeight);
    m_testNameBox->setFixedHeight(elHeight);
    m_testTimeBox->setFixedHeight(elHeight);
    m_questionCountBox->setFixedHeight(elHeight);
    m_addQuestions->setFixedSize(elWidth, elHeight);
    m_loadFromDoc->setFixedHeight(elHeight);
    m_saveinDb->setFixedHeight(elHeight);
    m_questionView->setFixedSize(elWidth, elHeight);

    m_questionCountBox->setValidator(new QRegExpValidator(QRegExp("\\d+"), this));

    m_box->setSpacing(20);

    m_box->addWidget(m_testName,      0, 0);
    m_box->addWidget(m_testTime,      1, 0);
    m_box->addWidget(m_questionCount, 2, 0);
    m_box->addWidget(m_loadFromDoc,   3, 0);
    m_box->addWidget(m_saveinDb,      4, 0);

    m_box->addWidget(m_testNameBox,      0, 1);
    m_box->addWidget(m_testTimeBox,      1, 1);
    m_box->addWidget(m_questionCountBox, 2, 1);
    m_box->addWidget(m_addQuestions,     3, 1);
    m_box->addWidget(m_questionView,     4, 1);

    setLayout(m_box);
}

void TestEditorSubView::loadTestFileData(const TestData &data)
{
    m_testNameBox->setText(data.testName);
    m_testTimeBox->setTime(data.testTime);
    m_questionCountBox->setText(QString::number(data.questionCount));
    emit questionCountLoadedFromFile(m_questionCountBox->text().toInt());
}

void TestEditorSubView::resize()
{
}

void TestEditorSubView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    resize();
}

void TestEditorSubView::addQuestions()
{
    if (m_questionCountBox->text().toInt() > 0)
        emit showSubView(QuestionEditor);
    else
        QMessageBox::warning(0, "Warning", "Сперва введите количество вопросов.");
}

void TestEditorSubView::loadDocFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Choose Test Document"), "", "DOC(*.doc *.docx)");
    emit loadedDocFile(filePath);
}

void TestEditorSubView::setTestName()
{
    emit testNameChanged(m_testNameBox->text());
}

void TestEditorSubView::setTestTime()
{
    emit testTimeChanged(m_testTimeBox->time());
}

void TestEditorSubView::setQuestionCount()
{
    emit questionCountChanged(m_questionCountBox->text().toInt());
}

void TestEditorSubView::showQuestionsListView()
{
    emit showSubView(QuestionViewer);
}
