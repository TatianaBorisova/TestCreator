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

protected:
    virtual void resize();

private slots:
    void back();

private:
    QGridLayout *m_box;

    QLabel *m_question;
    QLabel *m_weight;
    QLabel *m_image;

    QPlainTextEdit *m_questionBox;
    QLineEdit      *m_weightBox;

    QPushButton *m_back;
    QPushButton *m_next;
    QPushButton *m_loadImg;

};

#endif // QUESTIONEDITORSUBVIEW_H
