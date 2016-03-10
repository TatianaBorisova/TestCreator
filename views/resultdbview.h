#ifndef RESULTDBVIEW_H
#define RESULTDBVIEW_H

#include "testcreatorbaseview.h"

class ResultDbSubView;

class ResultDbView : public TestCreatorBaseView
{
    Q_OBJECT
public:
    explicit ResultDbView(QWidget *parent = 0);

    void setFixedSize(int w, int h);

public slots:
    void loadDbModel();

signals:
    void resultDbChanged(const QString &name);

protected:
    virtual void resize();

private:
    ResultDbSubView *m_resultView;
};

#endif // RESULTDBVIEW_H
