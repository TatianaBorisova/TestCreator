#include "testdbview.h"

#include <QListWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QTabWidget>
#include <QFileDialog>

namespace {
const QLatin1String slash = QLatin1String("/");
const int btnWidth = 300;
const int minHeight = 20;
}

TestDbView::TestDbView(QWidget *parent) :
    TestCreatorBaseView(parent),
    m_box(new QGridLayout(this)),
    m_vbox(new QVBoxLayout(this)),
    m_testBox(new QListWidget(this)),
    m_openTestFile(new QPushButton("Открыть тест", this)),
    m_createTestFile(new QPushButton("Создать новый тест", this)),
    m_loadTestFile(new QPushButton("Загрузить из Doc файла", this))
{
    connect(m_openTestFile, &QPushButton::clicked, this, &TestDbView::openExistedTest);
    connect(m_createTestFile, &QPushButton::clicked, this, &TestDbView::createNewTest);
    connect(m_loadTestFile, &QPushButton::clicked, this, &TestDbView::loadFromDocFile);

    QFont wdgFont("Times", 11);
    m_testBox->setFont(wdgFont);

    m_openTestFile->setFixedSize(btnWidth, 50);
    m_createTestFile->setFixedSize(btnWidth, 50);
    m_loadTestFile->setFixedSize(btnWidth, 50);

    m_vbox->addWidget(m_openTestFile);
    m_vbox->addWidget(m_loadTestFile);
    m_vbox->addWidget(m_createTestFile);

    m_box->setSpacing(minHeight);
    m_box->addWidget(m_testBox, 0, 0);
    m_box->addLayout(m_vbox, 0, 1);

    m_box->setMargin(minHeight);

    setLayout(m_box);
}

void TestDbView::resize()
{
    m_testBox->setFixedSize(width() - btnWidth - 3*minHeight, height()*0.8);
}

void TestDbView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    resize();
}

void TestDbView::loadFromDocFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Choose Test Document"), "", "Doc(*.doc, *.docx)");
    emit testFileName(filePath);
    emit showView(CreatorView);
}

void TestDbView::createNewTest()
{
    emit testFileName("");
    emit showView(CreatorView);
}

void TestDbView::openExistedTest()
{
    QString filename("");
    if (m_testBox->count() > 0) {
        QListWidgetItem *item = m_testBox->currentItem();
        if (item)
            filename = item->text();
    }
    emit testFileName(filename);
    emit showView(CreatorView);
}
