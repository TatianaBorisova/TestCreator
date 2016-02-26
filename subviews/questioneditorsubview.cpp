#include "questioneditorsubview.h"

#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QRegExpValidator>
#include <QFileDialog>

#include <QDebug>

namespace {
const int minHeight = 50;
}

QuestionEditorSubView::QuestionEditorSubView(QWidget *parent) :
    TestCreatorBaseView(parent),
    m_box(new QGridLayout(this)),
    m_question(new QLabel("Содержание вопроса № %1:", this)),
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
    connect(m_next, &QPushButton::clicked, this, &QuestionEditorSubView::next);
    connect(this, &QuestionEditorSubView::questionCounterChanged, this, &QuestionEditorSubView::setBackBtnText);

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

void QuestionEditorSubView::setQuestionsCount(int count)
{
    m_questionCounter = 0;
    m_questionMax = count;

    m_question->setText("Содержание вопроса № " + QString::number(m_questionCounter + 1));
}

void QuestionEditorSubView::updatedIndexData(const TestQuestions &test)
{
    m_questionBox->setPlainText(test.question);
    m_weightBox->setText(QString::number(test.weight));
    m_correctAnswerBox->setPlainText(test.answers.correctAnswer);
    m_incorrectAnswerBox->setPlainText(test.answers.uncorrectAnswers);
    m_filepath = test.answers.imgPath;
    if (!m_filepath.isEmpty()) {
        m_image->setPixmap(QPixmap(m_filepath).scaled(m_image->width(), m_image->height()));
    }
}

void QuestionEditorSubView::back()
{
    TestQuestions question;
    question.question = m_questionBox->toPlainText();
    question.weight = m_weightBox->text().toInt();
    question.answers.correctAnswer = m_correctAnswerBox->toPlainText();
    question.answers.uncorrectAnswers = m_incorrectAnswerBox->toPlainText();
    question.answers.imgPath = m_filepath;

    emit createdQuestion(question, questionCounter());
    clearQuestionArea();

    //if first or last element - return to TEST view
    if (questionCounter() == 0 /*|| questionCounter() == m_questionMax - 1*/) {

        setQuestionCounter(0);
        emit showSubView(TestEditor);

    } else {

        //else go to previous question entry
        setQuestionCounter(questionCounter() - 1);
    }
    m_question->setText("Содержание вопроса № " + QString::number(questionCounter() + 1));
}

void QuestionEditorSubView::loadImage()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Choose Test Document"), "", "DOC(*.jpg *.png *.bmp)");
    m_image->setPixmap(QPixmap(filePath).scaled(m_image->width(), m_image->height()));
    m_filepath = filePath;
}

void QuestionEditorSubView::next()
{
    TestQuestions question;
    question.question = m_questionBox->toPlainText();
    question.weight = m_weightBox->text().toInt();
    question.answers.correctAnswer = m_correctAnswerBox->toPlainText();
    question.answers.uncorrectAnswers = m_incorrectAnswerBox->toPlainText();
    question.answers.imgPath = m_filepath;

    emit createdQuestion(question, questionCounter());
    clearQuestionArea();

    //if last question - return to TEST view
    if (questionCounter() == m_questionMax - 1) {

        setQuestionCounter(0);
        emit showSubView(TestEditor);
        m_question->setText("Содержание вопроса № " + QString::number(questionCounter()));

    } else {
        //go to next question entry
        setQuestionCounter(questionCounter() + 1);
        m_question->setText("Содержание вопроса № " + QString::number(questionCounter() + 1));
    }
}

void QuestionEditorSubView::clearQuestionArea()
{
    m_questionBox->setPlainText("");
    m_weightBox->setText("");
    m_correctAnswerBox->setPlainText("");
    m_incorrectAnswerBox->setPlainText("");
    m_image->clear();
    m_filepath.clear();
}

int QuestionEditorSubView::questionCounter() const
{
    return m_questionCounter;
}

void QuestionEditorSubView::setQuestionCounter(int count)
{
    m_questionCounter = count;
    emit questionCounterChanged(count);
}

void QuestionEditorSubView::setBackBtnText(int index)
{
    if (index == 0) {
        m_back->setText("Cохранить и вернуться в начало");
    } else {
        m_back->setText("Сохранить и к предыдущему вопросу");
    }
    if (index == m_questionMax - 1) {
        m_next->setText("Cохранить и вернуться в начало");
    } else {
        m_next->setText("Сохранить и перейти в следующему");
    }
}
