#include "questioncreatortabview.h"
#include "subviews/questioneditorsubview.h"
#include "subviews/testeditorsubview.h"

QuestionCreatorTabView::QuestionCreatorTabView(QWidget *parent) :
    TestCreatorBaseView(parent),
    m_questionEditor(new QuestionEditorSubView(this)),
    m_testData(new TestEditorSubView(this))
{
    showSubView(TestEditor);
    connect(m_testData, SIGNAL(showSubView(SubViews)), this, SLOT(showSubView(SubViews)));
    connect(m_questionEditor, SIGNAL(showSubView(SubViews)), this, SLOT(showSubView(SubViews)));
}

void QuestionCreatorTabView::resize()
{
    m_questionEditor->setFixedSize(width(), height());
    m_testData->setFixedSize(width(), height());
}

void QuestionCreatorTabView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    resize();
}

void QuestionCreatorTabView::showSubView(SubViews view)
{
    m_questionEditor->hide();
    m_testData->hide();

    switch(view) {
    case QuestionEditor:
        m_questionEditor->show();
        break;
    case TestEditor:
        m_testData->show();
    default:
        break;
    }
}
