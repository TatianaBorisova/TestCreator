#include "questioneditorsubview.h"

#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>

namespace {
const int margin = 50;
}

QuestionEditorSubView::QuestionEditorSubView(QWidget *parent) :
    TestCreatorBaseView(parent),
    m_box(new QGridLayout(this)),
    m_question(new QLabel("Содержание вопроса:", this)),
    m_weight(new QLabel("Вес вопроса:", this)),
    m_image(new QLabel(this)),
    m_questionBox(new QLineEdit(this)),
    m_weightBox(new QLineEdit(this)),
    m_back(new QPushButton("Вернуться к параметрам теста", this)),
    m_next(new QPushButton("Заполнить следующий вопрос", this)),
    m_loadImg(new QPushButton("Загрузить изображение", this))
{
    connect(m_back, &QPushButton::clicked, this, &QuestionEditorSubView::back);

    QFont font("Times", 15);
    m_question->setFont(font);
    m_weight->setFont(font);
    m_questionBox->setFont(font);
    m_weightBox->setFont(font);
    m_back->setFont(font);
    m_next->setFont(font);
    m_loadImg->setFont(font);

    m_question->setFixedHeight(margin);
    m_weight->setFixedHeight(margin);
    m_questionBox->setFixedHeight(3*margin);
    m_weightBox->setFixedHeight(3*margin);
    m_back->setFixedHeight(margin);
    m_next->setFixedHeight(margin);
    m_loadImg->setFixedHeight(margin);
    m_image->setFixedSize(5*margin, 5*margin);

    m_box->setContentsMargins(margin, 2*margin, margin, margin);
    m_box->setSpacing(margin);

    m_box->addWidget(m_question, 0, 0);
    m_box->addWidget(m_questionBox, 1, 0);
    m_box->addWidget(m_weight, 2, 0);
    m_box->addWidget(m_weightBox, 3, 0);
    m_box->addWidget(m_back, 4, 0);

    m_box->addWidget(m_loadImg, 1, 1);
    m_box->addWidget(m_image, 2, 1);
    m_box->addWidget(m_next, 4, 1);

    setLayout(m_box);
}

void QuestionEditorSubView::resize()
{

}

void QuestionEditorSubView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    resize();
}

void QuestionEditorSubView::back()
{
    emit showSubView(TestEditor);
}
