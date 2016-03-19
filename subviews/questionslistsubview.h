#ifndef QUESTIONSLISTSUBVIEW_H
#define QUESTIONSLISTSUBVIEW_H

#include "views/testcreatorbaseview.h"
#include "global.h"

#include <QList>

class QPlainTextEdit;
class QPushButton;
class QHBoxLayout;

class QuestionsListSubView : public TestCreatorBaseView
{
    Q_OBJECT
public:
    explicit QuestionsListSubView(QWidget *parent = 0);

    void setFixedSize(int w, int h);

public slots:
    void setQuestionsList(const QList<TestQuestions> &list);

private slots:
    void back();

protected:
    virtual void resize();

private:
    QPlainTextEdit *m_questions;
    QPushButton    *m_back;
    QHBoxLayout    *m_box;

};

#endif // QUESTIONSLISTSUBVIEW_H
