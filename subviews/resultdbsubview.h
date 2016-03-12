#ifndef RESULTDBSUBVIEW_H
#define RESULTDBSUBVIEW_H

#include "views/testcreatorbaseview.h"
#include "global.h"

class QSqlQueryModel;
class QTableView;
class QGridLayout;
class QSqlDatabase;
class QLabel;
class QPushButton;
class QAxObject;

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

private slots:
    void saveToDocFile();
    void fillResultStructure();
    QString createDocFile();
    QString getTimeString(const QString time);

private:
    QSqlDatabase openDb();

    QSqlQueryModel *m_model;
    QTableView     *m_table;
    QString         m_dbname;
    QGridLayout    *m_box;
    QLabel         *m_header;
    QPushButton    *m_update;
    QPushButton    *m_docSaver;
    QAxObject      *m_wordApp;
    QAxObject      *m_wordDoc;
    QList<StudentResult> m_dbTable;
};

#endif // RESULTDBSUBVIEW_H
