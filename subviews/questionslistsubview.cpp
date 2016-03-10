#include "questionslistsubview.h"

#include <QPlainTextEdit>
#include <QPushButton>
#include <QPixmap>
#include <QHBoxLayout>

QuestionsListSubView::QuestionsListSubView(QWidget *parent) :
    TestCreatorBaseView(parent),
    m_questions(new QPlainTextEdit(this)),
    m_back(new QPushButton(this)),
    m_box(new QHBoxLayout(this))
{
    connect(m_back, &QPushButton::clicked, this, &QuestionsListSubView::back);

    m_questions->setReadOnly(true);

    m_back->setIcon(QIcon(QPixmap(":res/back")));
    m_back->setFixedSize(QSize(50, 50));
    m_back->setIconSize(QSize(m_back->width(), m_back->height()));

    m_box->addWidget(m_questions);
    m_box->addWidget(m_back);

    m_box->setAlignment(m_questions, Qt::AlignBottom);
    m_box->setAlignment(m_back, Qt::AlignBottom);

    m_box->setMargin(30);
    m_box->setSpacing(30);

    setLayout(m_box);
}

void QuestionsListSubView::resize()
{
    m_questions->setFixedSize(width() - 2*m_back->width() - 30, height() - 20);
}

void QuestionsListSubView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    resize();
}

void QuestionsListSubView::setQuestionsLost(const QList<TestQuestions> &list)
{
    m_questions->clear();

    for (int i = 0; i < list.count(); i++) {
        m_questions->setPlainText(m_questions->toPlainText() + "\n"
                                  + "\nВопрос: " + list.at(i).question + "\n"
                                  + "\nВерные ответы: " + list.at(i).answers.correctAnswer + "\n"
                                  + "\nНеверные ответы: " + list.at(i).answers.uncorrectAnswers + "\n"
                                  + "\nКартинка: " + list.at(i).answers.imgName + "\n");
    }
}

void QuestionsListSubView::back()
{
    emit showSubView(TestEditor);
}
