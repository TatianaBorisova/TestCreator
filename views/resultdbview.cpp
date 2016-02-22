#include "resultdbview.h"

ResultDbView::ResultDbView(QWidget *parent) :
    TestCreatorBaseView(parent)
{
}

void ResultDbView::resize()
{
}

void ResultDbView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    resize();
}
