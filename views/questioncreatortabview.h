#ifndef QUESTIONCREATORTABVIEW_H
#define QUESTIONCREATORTABVIEW_H

#include "testcreatorbaseview.h"

class QuestionEditorSubView;
class TestEditorSubView;

class QuestionCreatorTabView : public TestCreatorBaseView
{
    Q_OBJECT

public:
    explicit QuestionCreatorTabView(QWidget *parent = 0);

    void setFixedSize(int w, int h);

protected slots:
    void showSubView(SubViews view);

protected:
    virtual void resize();

private:
    QuestionEditorSubView *m_questionEditor;
    TestEditorSubView     *m_testData;
};

#endif // QUESTIONCREATORTABVIEW_H
