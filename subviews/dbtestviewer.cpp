#include "dbtestviewer.h"

#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>

DbTestViewer::DbTestViewer(QWidget *parent) :
    TestCreatorBaseView(parent),
    m_dbTests(new QListWidget(this)),
    m_hbox(new QHBoxLayout(this)),
    m_btn(new QPushButton("Выбрать тест", this))
{
    connect(m_btn, &QPushButton::clicked, this, &DbTestViewer::chosenTestname);

    m_btn->setFixedHeight(50);

    m_hbox->addWidget(m_dbTests);
    m_hbox->addWidget(m_btn);

    m_hbox->setMargin(30);
    m_hbox->setSpacing(30);

    setLayout(m_hbox);
}

void DbTestViewer::resize()
{
    m_dbTests->setFixedSize(width()*0.9 - m_btn->width(), height()*0.9);
}

void DbTestViewer::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    resize();
}

void DbTestViewer::fillTestVariants(const QList<TestHeaderData> &test)
{
    if (test.count() > 0)
        m_dbTests->clear();

    for (int i = 0; i < test.count(); i++) {
        QListWidgetItem *newItem = new QListWidgetItem();

        newItem->setText(test.at(i).testName);
        m_dbTests->insertItem(m_dbTests->count(), newItem);
    }
}

void DbTestViewer::chosenTestname()
{
    if (m_dbTests->count() > 0) {
        QListWidgetItem *item = m_dbTests->currentItem();
        if (item)
            emit chosenTest(item->text());
        else
            QMessageBox::warning(0, "Ошибка", "Выберите тест из списка.");
    }
}
