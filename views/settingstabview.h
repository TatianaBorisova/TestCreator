#ifndef SETTINGSTABVIEW_H
#define SETTINGSTABVIEW_H

#include "testcreatorbaseview.h"

class SettingsTabView : public TestCreatorBaseView
{
    Q_OBJECT
public:
    explicit SettingsTabView(QWidget *parent = 0);

    void setFixedSize(int w, int h);

protected:
    virtual void resize();
};

#endif // SETTINGSTABVIEW_H
