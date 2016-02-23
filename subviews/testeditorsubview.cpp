#include "testeditorsubview.h"

#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QRegExpValidator>
#include <QTimeEdit>
#include <QMessageBox>

namespace {
const int elHeight = 50;
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
    m_addQuestions(new QPushButton("Добавить вопросы", this)),
    m_loadFromDoc(new QPushButton("Загрузить тест из Doc файла", this))
{
    connect(m_addQuestions, &QPushButton::clicked, this, &TestEditorSubView::addQuestions);
    connect(m_testNameBox, &QLineEdit::editingFinished, this, &TestEditorSubView::setTestName);
    connect(m_testTimeBox, &QTimeEdit::editingFinished, this, &TestEditorSubView::setTestTime);
    connect(m_questionCountBox, &QLineEdit::editingFinished, this, &TestEditorSubView::setQuestionCount);

    m_testName->setFixedHeight(elHeight);
    m_testTime->setFixedHeight(elHeight);
    m_questionCount->setFixedHeight(elHeight);
    m_testNameBox->setFixedHeight(elHeight);
    m_testTimeBox->setFixedHeight(elHeight);
    m_questionCountBox->setFixedHeight(elHeight);
    m_addQuestions->setFixedHeight(elHeight);
    m_loadFromDoc->setFixedHeight(elHeight);

    m_questionCountBox->setValidator(new QRegExpValidator(QRegExp("\\d+"), this));

    m_box->setSpacing(20);

    m_box->addWidget(m_testName, 0, 0);
    m_box->addWidget(m_testTime, 1, 0);
    m_box->addWidget(m_questionCount, 2, 0);
    m_box->addWidget(m_addQuestions, 3, 0);
    m_box->addWidget(m_loadFromDoc, 4, 0);

    m_box->addWidget(m_testNameBox, 0, 1);
    m_box->addWidget(m_testTimeBox, 1, 1);
    m_box->addWidget(m_questionCountBox, 2, 1);

    setLayout(m_box);
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
