#ifndef RESULTDBSUBVIEW_H
#define RESULTDBSUBVIEW_H

#include "views/testcreatorbaseview.h"

class QSqlQueryModel;
class QTableView;
class QGridLayout;
class QSqlDatabase;
class QLabel;
class QPushButton;

class ResultDbSubView : public TestCreatorBaseView
{
    Q_OBJECT
public:
    explicit ResultDbSubView(QWidget *parent = 0);

    void setFixedSize(int w, int h);

public slots:
    void setDbName(const QString &name);
    void loadDbModel();

protected:
    virtual void resize();

private:
    QSqlDatabase openDb();

private:
    QSqlQueryModel *m_model;
    QTableView     *m_table;
    QString         m_dbname;
    QGridLayout    *m_box;
    QLabel         *m_header;
    QPushButton    *m_update;
};

#endif // RESULTDBSUBVIEW_H
