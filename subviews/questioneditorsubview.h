#ifndef QUESTIONEDITORSUBVIEW_H
#define QUESTIONEDITORSUBVIEW_H

#include "views/testcreatorbaseview.h"

class QPlainTextEdit;
class QLabel;
class QLineEdit;
class QPushButton;
class QGridLayout;

class QuestionEditorSubView : public TestCreatorBaseView
{
    Q_OBJECT
public:
    explicit QuestionEditorSubView(QWidget *parent = 0);

    void setFixedSize(int w, int h);

signals:
    void createdQuestion(const TestQuestions &test, int index);

protected:
    virtual void resize();

private slots:
    void back();
    void loadImage();
    void next();

private:
    QGridLayout *m_box;

    QLabel *m_question;
    QLabel *m_weight;
    QLabel *m_image;
    QLabel *m_correctAnswer;
    QLabel *m_incorrectAnswer;

    QPlainTextEdit *m_questionBox;
    QLineEdit      *m_weightBox;
    QPlainTextEdit *m_correctAnswerBox;
    QPlainTextEdit *m_incorrectAnswerBox;

    QPushButton *m_back;
    QPushButton *m_next;
    QPushButton *m_loadImg;

    QString m_filepath;
    int m_questionCounter;

};

#endif // QUESTIONEDITORSUBVIEW_H
