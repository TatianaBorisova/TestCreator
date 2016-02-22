#ifndef TESTCREATORBASEVIEW_H
#define TESTCREATORBASEVIEW_H

#include "global.h"

#include <QWidget>

class TestCreatorBaseView : public QWidget
{
    Q_OBJECT
public:
    explicit TestCreatorBaseView(QWidget *parent = 0);

signals:
    void showView(TestViews view);
    void showSubView(SubViews view);

public slots:
    void show();

protected:
    virtual void resize()= 0;
};

#endif // TESTCREATORBASEVIEW_H
