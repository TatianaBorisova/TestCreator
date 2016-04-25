#include "testcreatorbaseview.h"

TestCreatorBaseView::TestCreatorBaseView(QWidget *parent) :
    QWidget(parent)
{

}

void TestCreatorBaseView::show()
{
    resize();
    QWidget::show();
}

void TestCreatorBaseView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resize();
}
