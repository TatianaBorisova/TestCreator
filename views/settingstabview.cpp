#include "settingstabview.h"

SettingsTabView::SettingsTabView(QWidget *parent) :
    TestCreatorBaseView(parent)
{

}

void SettingsTabView::resize()
{

}

void SettingsTabView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    resize();
}

