#include "settingstabview.h"

#include "subviews/settingsubview.h"

SettingsTabView::SettingsTabView(QWidget *parent) :
    TestCreatorBaseView(parent),
    m_settings(new SettingSubView(this))
{
    connect(m_settings, &SettingSubView::testDbChanged, this, &SettingsTabView::testDbChanged);
    connect(m_settings, &SettingSubView::resultDbChanged, this, &SettingsTabView::resultDbChanged);
    connect(this, &SettingsTabView::testFolderPathChanged, m_settings, &SettingSubView::testFolderPathChanged);
}

void SettingsTabView::resize()
{
    m_settings->setFixedSize(width(), height());
}

void SettingsTabView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    resize();
}

