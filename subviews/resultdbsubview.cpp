#include "resultdbsubview.h"

#include <QTableView>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlTableModel>
#include <QGridLayout>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QLabel>
#include <QDebug>
#include <QPushButton>
#include <QMutex>
#include <QMutexLocker>
#include <QFileDialog>
#include <QAxObject>

namespace {
const int margin = 50;
}

ResultDbSubView::ResultDbSubView(QWidget *parent) :
    TestCreatorBaseView(parent),
    m_model(new QSqlQueryModel(this)),
    m_table(new QTableView(this)),
    m_box(new QGridLayout(this)),
    m_header(new QLabel("Таблица результатов тестирования:", this)),
    m_update(new QPushButton("Обновить таблицу", this)),
    m_docSaver(new QPushButton("Выгрузить в Doc файл", this))
{
    connect(m_update, &QPushButton::clicked, this, &ResultDbSubView::loadDbModel);
    connect(m_docSaver, &QPushButton::clicked, this, &ResultDbSubView::saveToDocFile);

    m_table->setModel(m_model);

    m_box->addWidget(m_header, 0, 0);
    m_box->addWidget(m_update, 0, 1);
    m_box->addWidget(m_docSaver, 2, 1);
    m_box->addWidget(m_table, 3, 0);

    m_box->setAlignment(m_update, Qt::AlignRight);
    m_box->setAlignment(m_docSaver, Qt::AlignRight);
    m_box->setMargin(margin);

    setLayout(m_box);
}

void ResultDbSubView::resize()
{
    m_table->setFixedSize(width() - 2*margin, height()*0.7);
    m_header->setFixedSize(width()*0.7, margin);
    m_update->setFixedWidth(200);
    m_docSaver->setFixedWidth(200);
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

    //make table full screen
    for (int i = 0; i < m_model->columnCount(); i++) {
        m_table->setColumnWidth(i, m_table->width()/m_model->columnCount());
    }
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

void ResultDbSubView::fillResultStructure()
{
    m_dbTable.clear();

    QSqlDatabase db = openDb();
    if (db.isOpen()) {

        QSqlQuery q_select(db);
        q_select.prepare("SELECT * FROM studentresults");

        if (q_select.exec()) {
            while (q_select.next()) {
                //testname, firstname, secondName, surname, groupname, scorevalue, maxvalue, testtime
                StudentResult table;
                table.id              = q_select.value(q_select.record().indexOf("id")).toInt();
                table.firstName       = q_select.value(q_select.record().indexOf("firstname")).toString();
                table.secondName      = q_select.value(q_select.record().indexOf("secondName")).toString();
                table.surname         = q_select.value(q_select.record().indexOf("surname")).toString();
                table.group           = q_select.value(q_select.record().indexOf("groupname")).toString();
                table.score           = q_select.value(q_select.record().indexOf("scorevalue")).toInt();
                table.maxPosibleScore = q_select.value(q_select.record().indexOf("maxvalue")).toInt();
                table.time            = getTimeString(q_select.value(q_select.record().indexOf("testtime")).toString());
                table.testName        = q_select.value(q_select.record().indexOf("testname")).toString();
                m_dbTable.append(table);
            }
        }
    }
    db.close();
    //    for (int i = 0; i < m_dbTable.count(); i++) {
    //        qDebug() << m_dbTable.at(i).id;
    //        qDebug() << m_dbTable.at(i).testName;
    //        qDebug() << m_dbTable.at(i).firstName;
    //        qDebug() << m_dbTable.at(i).secondName;
    //        qDebug() << m_dbTable.at(i).surname;
    //        qDebug() << m_dbTable.at(i).group;
    //        qDebug() << m_dbTable.at(i).score;
    //        qDebug() << m_dbTable.at(i).maxPosibleScore;
    //        qDebug() << m_dbTable.at(i).time;
    //    }
}

QString ResultDbSubView::getTimeString(const QString time)
{
    QString result = time;
    result = result.replace("T", " ");
    return result;
}

void ResultDbSubView::saveToDocFile()
{
    fillResultStructure();

    if (m_dbTable.count() > 0) {
        QString file = createDocFile();

        m_wordApp = new QAxObject("Word.Application",this);
        m_wordDoc = m_wordApp->querySubObject("Documents()");
        m_wordDoc->dynamicCall("Open(QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant)", file, false, false, false, "", "", false);  //querySubObject("Add()");
        m_wordApp->setProperty("Visible", true);

        //Get active document.
        QAxObject* pActiveDocument = m_wordApp->querySubObject("ActiveDocument()");

        //Get selection.
        QAxObject *pSelection = m_wordApp->querySubObject("ActiveDocument")->querySubObject("ActiveWindow")->querySubObject("Selection");

        // создание таблицы
        QAxObject* pTables = pActiveDocument->querySubObject("Tables()");
        QAxObject* pNewTable = pTables->querySubObject("Add(Id, testname, firstname, secondName, surname, groupname, scorevalue, maxvalue, testtime)", pSelection->property("Range"), 1, 8, 1, 1);

        //Resize table width to whole page width.
//        pNewTable->setProperty("PreferredWidthType", "wdPreferredWidthPercent");
//        pNewTable->setProperty("PreferredWidth", 105);

        //Align table to center.
        pNewTable->querySubObject("Rows()")->setProperty("Alignment", "wdAlignRowCenter");

        //Iterate found records.
        QAxObject *pCell = NULL, *pCellRange = NULL;
        for(int cur_row = 0; cur_row < m_dbTable.count(); cur_row++)
        {
            //Inserting new row for each new data.
            pSelection->dynamicCall("InsertRowsBelow()");
        }

        for (int cur_row = 0; cur_row < m_dbTable.count(); cur_row++) {

            pCell = pNewTable->querySubObject("Cell(Row, Column)", cur_row, 1);
            pCellRange = pCell->querySubObject("Range()");
            pCellRange->dynamicCall("InsertAfter(Text)", m_dbTable.at(cur_row).testName);

            pCell = pNewTable->querySubObject("Cell(Row, Column)", cur_row, 2);
            pCellRange = pCell->querySubObject("Range()");
            pCellRange->dynamicCall("InsertAfter(Text)", m_dbTable.at(cur_row).firstName);

            pCell = pNewTable->querySubObject("Cell(Row, Column)", cur_row, 3);
            pCellRange = pCell->querySubObject("Range()");
            pCellRange->dynamicCall("InsertAfter(Text)", m_dbTable.at(cur_row).secondName);

            pCell = pNewTable->querySubObject("Cell(Row, Column)", cur_row, 4);
            pCellRange = pCell->querySubObject("Range()");
            pCellRange->dynamicCall("InsertAfter(Text)", m_dbTable.at(cur_row).surname);

            pCell = pNewTable->querySubObject("Cell(Row, Column)", cur_row, 5);
            pCellRange = pCell->querySubObject("Range()");
            pCellRange->dynamicCall("InsertAfter(Text)", m_dbTable.at(cur_row).group);

            pCell = pNewTable->querySubObject("Cell(Row, Column)", cur_row, 6);
            pCellRange = pCell->querySubObject("Range()");
            pCellRange->dynamicCall("InsertAfter(Text)", m_dbTable.at(cur_row).score);

            pCell = pNewTable->querySubObject("Cell(Row, Column)", cur_row, 7);
            pCellRange = pCell->querySubObject("Range()");
            pCellRange->dynamicCall("InsertAfter(Text)", m_dbTable.at(cur_row).maxPosibleScore);

            pCell = pNewTable->querySubObject("Cell(Row, Column)", cur_row, 8);
            pCellRange = pCell->querySubObject("Range()");
            pCellRange->dynamicCall("InsertAfter(Text)", m_dbTable.at(cur_row).time);
        }
    } else {
        QMessageBox::warning(0, "Warning", "В выбранной Вами базе нет данных.");
    }
    // Iterate found records.End.
}

QString ResultDbSubView::createDocFile()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    "Save File",
                                                    "TEST.doc",
                                                    "Documents (*.doc *.docx)");

    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    file.close();

    return file.fileName();
}
