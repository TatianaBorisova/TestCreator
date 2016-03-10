#include "resultdbsubview.h"

#include <QTableView>
#include <QSqlQueryModel>
#include <QGridLayout>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QLabel>
#include <QDebug>
#include <QPushButton>
#include <QMutex>
#include <QMutexLocker>

namespace {
const int margin = 50;
}

ResultDbSubView::ResultDbSubView(QWidget *parent) :
    TestCreatorBaseView(parent),
    m_model(new QSqlQueryModel(this)),
    m_table(new QTableView(this)),
    m_box(new QGridLayout(this)),
    m_header(new QLabel("Таблица результатов тестирования:", this)),
    m_update(new QPushButton("Обновить", this))
{
    connect(m_update, &QPushButton::clicked, this, &ResultDbSubView::loadDbModel);

    m_table->setModel(m_model);

    m_box->addWidget(m_header, 0, 0);
    m_box->addWidget(m_update, 0, 1);
    m_box->addWidget(m_table, 1, 0);

    m_box->setAlignment(m_update, Qt::AlignRight);
    m_box->setMargin(margin);

    setLayout(m_box);
}

void ResultDbSubView::resize()
{
    m_table->setFixedSize(width() - 2*margin, height()*0.7);
    m_header->setFixedSize(width()*0.7, margin);
}

void ResultDbSubView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    resize();
}

void ResultDbSubView::setDbName(const QString &name)
{
    qDebug() << "changed to " << name;
    m_dbname = name;
    loadDbModel();
}

void ResultDbSubView::loadDbModel()
{
    QMutex mutex;
    QMutexLocker locker(&mutex);

    QSqlDatabase db = openDb();
    if (db.isOpen()) {
        m_model->setQuery("SELECT * FROM studentresults", db);
    }
    db.close();
}

QSqlDatabase ResultDbSubView::openDb()
{
    QSqlDatabase dbPtr = QSqlDatabase::addDatabase("QSQLITE");

    if (m_dbname.isEmpty()) {
        QMessageBox::critical(0, "Can not open database", "Введите имя базы данных на вкладке настройки.");
        return dbPtr;
    }

    dbPtr.setDatabaseName(m_dbname);
    if (!dbPtr.open()) {
        QMessageBox::critical(0, "Can not open database", "Не могу открыть " + m_dbname + " базу данных.\n");
    }

    return dbPtr;
}
