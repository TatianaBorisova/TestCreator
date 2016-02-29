#ifndef DBTESTVIEWER_H
#define DBTESTVIEWER_H

#include "views/testcreatorbaseview.h"

class QListWidget;
class QHBoxLayout;
class QPushButton;

class DbTestViewer : public TestCreatorBaseView
{
    Q_OBJECT
public:
    explicit DbTestViewer(QWidget *parent = 0);

    void setFixedSize(int w, int h);

signals:
    void chosenTest(const QString &testname);

public slots:
    void fillTestVariants(const QList<TestHeaderData> &test);

protected slots:
    void chosenTestname();

protected:
    virtual void resize();

private:
    QListWidget *m_dbTests;
    QHBoxLayout *m_hbox;
    QPushButton *m_btn;
};

#endif // DBTESTVIEWER_H
