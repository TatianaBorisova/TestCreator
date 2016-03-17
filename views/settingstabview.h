#ifndef SETTINGSTABVIEW_H
#define SETTINGSTABVIEW_H

#include "testcreatorbaseview.h"

class SettingSubView;

class SettingsTabView : public TestCreatorBaseView
{
    Q_OBJECT
public:
    explicit SettingsTabView(QWidget *parent = 0);

    void setFixedSize(int w, int h);

signals:
    void testDbChanged(const QString &value);
    void resultDbChanged(const QString &value);
    void testFolderPathChanged(const QString &path);

protected:
    virtual void resize();

private:
    SettingSubView *m_settings;
};

#endif // SETTINGSTABVIEW_H
