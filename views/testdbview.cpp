#include "testdbview.h"

#include <QListWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QTabWidget>
#include <QFileDialog>
#include <QComboBox>

#include <QDebug>

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
    m_folderContainer(new QPushButton("Выбрать папку", this))
{
    connect(m_openTestFile, &QPushButton::clicked, this, &TestDbView::openExistedTest);
    connect(m_createTestFile, &QPushButton::clicked, this, &TestDbView::createNewTest);
    connect(m_folderContainer, &QPushButton::clicked, this, &TestDbView::loadFromFolder);

    QFont wdgFont("Times", 11);
    m_testBox->setFont(wdgFont);

    m_openTestFile->setFixedSize(btnWidth, 50);
    m_createTestFile->setFixedSize(btnWidth, 50);
    m_folderContainer->setFixedSize(btnWidth, 50);

    m_vbox->addWidget(m_folderContainer);
    m_vbox->addWidget(m_openTestFile);
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

void TestDbView::loadFromFolder()
{
    QString filePath = QFileDialog::getExistingDirectory(this, tr("Open Directory"));

    if (!filePath.isEmpty()) {

        m_testBox->clear();
        QDir chosenDir(filePath);

        QStringList fileFilter;
        QStringList allFiles = chosenDir.entryList(fileFilter);

        qDebug() << "allFiles = " << allFiles.count() << filePath;
        for (int i = 0; i < allFiles.count(); i++) {
            if (!findDumlicateFile(m_testBox, allFiles.at(i))) {

                QListWidgetItem *newItem = new QListWidgetItem();

                newItem->setText(filePath + allFiles.at(i));
                m_testBox->insertItem(m_testBox->count(), newItem);
            }
        }
    }
}

void TestDbView::createNewTest()
{
    emit showView(CreatorView);
}

void TestDbView::openExistedTest()
{
    QString filename("");
    if (m_testBox->count() > 0) {
        QListWidgetItem *item = m_testBox->currentItem();
        if (item) {
            filename = item->text();
            QFile file(filename);

            if (file.exists()) {
                QFileInfo fileExt(filename);
                QString ext = fileExt.suffix();
                if (ext == ".doc" || ext == ".docx") {
                    emit docFileName(filename);
                } else {
                    emit bdFileName(filename);
                }
            }
        }
    }
    emit showView(CreatorView);
}

bool TestDbView::findDumlicateFile(QListWidget *itemBox, const QString &fileName)
{
    if (itemBox) {
        for (int i = 0; i < itemBox->count(); i++) {
            if (fileName == itemBox->item(i)->text())
                return true;
        }
    }
    return false;
}

