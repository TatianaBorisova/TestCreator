#ifndef TESTEDITORSUBVIEW_H
#define TESTEDITORSUBVIEW_H

#include "views/testcreatorbaseview.h"

class QLabel;
class QLineEdit;
class QPushButton;
class QGridLayout;

class TestEditorSubView : public TestCreatorBaseView
{
    Q_OBJECT
public:
    explicit TestEditorSubView(QWidget *parent = 0);
    void setFixedSize(int w, int h);

protected:
    virtual void resize();

private slots:
    void addQuestions();

private:
    QGridLayout *m_box;
    QLabel *m_testName;
    QLabel *m_testTime;
    QLabel *m_questionCount;
    QLineEdit *m_testNameBox;
    QLineEdit *m_testTimeBox;
    QLineEdit *m_questionCountBox;
    QPushButton *m_addQuestions;
    QPushButton *m_loadFromDoc;
};

#endif // TESTEDITORSUBVIEW_H
