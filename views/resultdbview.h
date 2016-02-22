#ifndef RESULTDBVIEW_H
#define RESULTDBVIEW_H

#include "testcreatorbaseview.h"

class ResultDbView : public TestCreatorBaseView
{
    Q_OBJECT
public:
    explicit ResultDbView(QWidget *parent = 0);

    void setFixedSize(int w, int h);

protected:
    virtual void resize();
};

#endif // RESULTDBVIEW_H
