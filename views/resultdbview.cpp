#include "resultdbview.h"

#include "subviews/resultdbsubview.h"

ResultDbView::ResultDbView(QWidget *parent) :
    TestCreatorBaseView(parent),
    m_resultView(new ResultDbSubView(this))
{
    connect(this, &ResultDbView::resultDbChanged, m_resultView, &ResultDbSubView::setDbName);
}

void ResultDbView::resize()
{
    m_resultView->setFixedSize(width(), height());
}

void ResultDbView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    resize();
}

 void ResultDbView::loadDbModel()
 {
     m_resultView->loadDbModel();
 }
