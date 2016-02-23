#include "questioneditorsubview.h"

#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QRegExpValidator>
#include <QFileDialog>

namespace {
const int minHeight = 50;
}

QuestionEditorSubView::QuestionEditorSubView(QWidget *parent) :
    TestCreatorBaseView(parent),
    m_box(new QGridLayout(this)),
    m_question(new QLabel("Содержание вопроса:", this)),
    m_weight(new QLabel("Вес вопроса:", this)),
    m_image(new QLabel(this)),
    m_correctAnswer(new QLabel("Верный ответ:", this)),
    m_incorrectAnswer(new QLabel("Перечислите неверные ответы через ';'", this)),
    m_questionBox(new QPlainTextEdit(this)),
    m_weightBox(new QLineEdit(this)),
    m_correctAnswerBox(new QPlainTextEdit(this)),
    m_incorrectAnswerBox(new QPlainTextEdit(this)),
    m_back(new QPushButton("Вернуться к параметрам теста", this)),
    m_next(new QPushButton("Сохранить и перейти в следующему", this)),
    m_loadImg(new QPushButton("Загрузить изображение", this))
{
    this->setStyleSheet("QPushButton { min-height: 50px; }");

    connect(m_back, &QPushButton::clicked, this, &QuestionEditorSubView::back);
    connect(m_loadImg, &QPushButton::clicked, this, &QuestionEditorSubView::loadImage);

    m_weight->setAlignment(Qt::AlignBottom);
    m_question->setAlignment(Qt::AlignBottom);

    m_image->setStyleSheet("QLabel { background-color : white; border: 2px solid grey }");

    m_weightBox->setValidator(new QRegExpValidator(QRegExp("\\d+"), this));
    m_weightBox->setMaxLength(2);

    m_box->setContentsMargins(minHeight, minHeight/2, minHeight, minHeight/10);
    m_box->setSpacing(minHeight);

    m_box->addWidget(m_question, 0, 0);
    m_box->addWidget(m_questionBox, 1, 0);
    m_box->addWidget(m_weight, 2, 0);
    m_box->addWidget(m_weightBox, 3, 0);
    m_box->addWidget(m_correctAnswer, 4, 0);
    m_box->addWidget(m_correctAnswerBox, 5, 0);
    m_box->addWidget(m_incorrectAnswer, 6, 0);
    m_box->addWidget(m_incorrectAnswerBox, 7, 0);
    m_box->addWidget(m_back, 8, 0);

    m_box->addWidget(m_image, 0, 1);
    m_box->addWidget(m_loadImg, 3, 1);
    m_box->addWidget(m_next, 8, 1);

    m_box->setAlignment(m_back, Qt::AlignRight);
    m_box->setAlignment(m_image, Qt::AlignTop);
    m_box->setAlignment(m_loadImg, Qt::AlignBottom);
    m_box->setAlignment(m_question, Qt::AlignBottom);
    m_box->setAlignment(m_weight, Qt::AlignBottom);

    setLayout(m_box);
}

void QuestionEditorSubView::resize()
{
    m_image->setFixedSize(height()*0.3, height()*0.3);
}

void QuestionEditorSubView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    resize();
}

void QuestionEditorSubView::back()
{
    if (m_questionCounter == 0)
        emit showSubView(TestEditor);
    else
        m_questionCounter--;
}

void QuestionEditorSubView::loadImage()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Choose Test Document"), "", "DOC(*.jpg *.png *.bmp)");
    m_image->setPixmap(QPixmap(filePath).scaled(m_image->width(), m_image->height()));
    m_filepath = filePath;
}

void QuestionEditorSubView::next()
{
    TestQuestions test;
    test.question = m_questionBox->toPlainText();
    test.weight = m_weightBox->text().toInt();
    test.answers.correctAnswer = m_correctAnswerBox->toPlainText();
    test.answers.uncorrectAnswers = m_incorrectAnswerBox->toPlainText();
    test.answers.imgPath = m_filepath;

    emit createdQuestion(test, m_questionCounter);
    m_questionCounter++;
}
