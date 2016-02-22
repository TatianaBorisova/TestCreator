#include "testeditorsubview.h"

#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QIntValidator>

TestEditorSubView::TestEditorSubView(QWidget *parent) :
    TestCreatorBaseView(parent),
    m_box(new QGridLayout(this)),
    m_testName(new QLabel("Название теста", this)),
    m_testTime(new QLabel("Время выполнения теста", this)),
    m_questionCount(new QLabel("Количество вопросов", this)),
    m_testNameBox(new QLineEdit(this)),
    m_testTimeBox(new QLineEdit(this)),
    m_questionCountBox(new QLineEdit(this)),
    m_addQuestions(new QPushButton("Добавить вопросы", this)),
    m_loadFromDoc(new QPushButton("Загрузить вопросы из Doc файла", this))
{
    connect(m_addQuestions, &QPushButton::clicked, this, &TestEditorSubView::addQuestions);

    QFont font("Times", 15);
    m_testName->setFont(font);
    m_testTime->setFont(font);
    m_questionCount->setFont(font);
    m_testNameBox->setFont(font);
    m_testTimeBox->setFont(font);
    m_questionCountBox->setFont(font);
    m_addQuestions->setFont(font);
    m_loadFromDoc->setFont(font);

    m_testName->setFixedHeight(50);
    m_testTime->setFixedHeight(50);
    m_questionCount->setFixedHeight(50);
    m_testNameBox->setFixedHeight(50);
    m_testTimeBox->setFixedHeight(50);
    m_questionCountBox->setFixedHeight(50);
    m_addQuestions->setFixedHeight(50);
    m_loadFromDoc->setFixedHeight(50);

    m_questionCountBox->setAlignment(Qt::AlignRight);
    m_questionCountBox->setValidator(new QIntValidator(this));

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
    emit showSubView(QuestionEditor);
}
