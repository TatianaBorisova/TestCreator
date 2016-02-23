#include "questioneditorsubview.h"

#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QRegExpValidator>

namespace {
const int minHeight = 50;
}

QuestionEditorSubView::QuestionEditorSubView(QWidget *parent) :
    TestCreatorBaseView(parent),
    m_box(new QGridLayout(this)),
    m_question(new QLabel("Содержание вопроса:", this)),
    m_weight(new QLabel("Вес вопроса:", this)),
    m_image(new QLabel(this)),
    m_questionBox(new QPlainTextEdit(this)),
    m_weightBox(new QLineEdit(this)),
    m_back(new QPushButton("Вернуться к параметрам теста", this)),
    m_next(new QPushButton("Заполнить следующий вопрос", this)),
    m_loadImg(new QPushButton("Загрузить изображение", this))
{
    connect(m_back, &QPushButton::clicked, this, &QuestionEditorSubView::back);

    QFont font;
    font.setPixelSize(15);
    m_question->setFont(font);
    m_weight->setFont(font);
    m_questionBox->setFont(font);
    m_weightBox->setFont(font);
    m_back->setFont(font);
    m_next->setFont(font);
    m_loadImg->setFont(font);

    m_weight->setAlignment(Qt::AlignBottom);
    m_question->setAlignment(Qt::AlignBottom);

    m_question->setFixedHeight(5*minHeight);
    m_weight->setFixedHeight(minHeight);
    m_questionBox->setFixedHeight(3*minHeight);
    m_weightBox->setFixedHeight(3*minHeight);
    m_back->setFixedSize(6*minHeight, minHeight);
    m_next->setFixedHeight(minHeight);
    m_loadImg->setFixedHeight(minHeight);
    m_image->setFixedSize(5*minHeight, 5*minHeight);

    m_image->setStyleSheet("QLabel { background-color : white; border: 2px solid grey }");

    m_weightBox->setValidator(new QRegExpValidator(QRegExp("\\d+"), this));
    m_box->setContentsMargins(minHeight, minHeight/2, minHeight, minHeight/10);
    m_box->setSpacing(minHeight);

    m_box->addWidget(m_question, 0, 0);
    m_box->addWidget(m_questionBox, 1, 0);
    m_box->addWidget(m_weight, 2, 0);
    m_box->addWidget(m_weightBox, 3, 0);
    m_box->addWidget(m_back, 4, 0);

    m_box->addWidget(m_image, 0, 1);
    m_box->addWidget(m_loadImg, 1, 1);
    m_box->addWidget(m_next, 4, 1);

    m_box->setAlignment(m_back, Qt::AlignRight);
    m_box->setAlignment(m_image, Qt::AlignTop);
    m_box->setAlignment(m_loadImg, Qt::AlignBottom);
    m_box->setAlignment(m_question, Qt::AlignBottom);
    m_box->setAlignment(m_weight, Qt::AlignBottom);

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
