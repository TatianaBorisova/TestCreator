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
