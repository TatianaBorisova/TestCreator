#include "testdbview.h"
#include "sqldatabase.h"

#include <QListWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QTabWidget>
#include <QFileDialog>
#include <QComboBox>
#include <QLabel>
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
    m_hbox(new QHBoxLayout(this)),
    m_testBox(new QListWidget(this)),
    m_openTestFile(new QPushButton("Открыть тест", this)),
    m_createTestFile(new QPushButton("Создать новый тест", this)),
    m_folderContainer(new QPushButton("Выбрать папку", this)),
    m_header(new QLabel("Выбранная папка:", this)),
    m_chosenFolder(new QLabel(this))
{
    connect(m_openTestFile, &QPushButton::clicked, this, &TestDbView::openExistedTest);
    connect(m_createTestFile, &QPushButton::clicked, this, &TestDbView::createNewTest);
    connect(m_folderContainer, &QPushButton::clicked, this, &TestDbView::loadFromFolder);

    QFont wdgFont("Times", 11);
    m_testBox->setFont(wdgFont);

    m_vbox->addWidget(m_folderContainer);
    m_vbox->addWidget(m_openTestFile);
    m_vbox->addWidget(m_createTestFile);

    m_hbox->addWidget(m_header);
    m_hbox->addWidget(m_chosenFolder);

    m_box->setSpacing(minHeight);
    m_box->addLayout(m_hbox, 0, 0);
    m_box->addWidget(m_testBox, 1, 0);
    m_box->addLayout(m_vbox, 1, 1);

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

        filePath = filePath + "/";

        setTestFolderPath(filePath);
        m_chosenFolder->setText(filePath);
        m_testBox->clear();

        QDir chosenDir(testFolderPath());
        QStringList allFiles = chosenDir.entryList(QDir::Files | QDir::NoDotAndDotDot);

        for (int i = 0; i < allFiles.count(); i++) {

            QFileInfo fileExt(allFiles.at(i));
            QString ext = fileExt.suffix().toLower();

            if (ext == ".doc" || ext == ".docx" || SqlliteDbManager::checkIfTestDb(allFiles.at(i))) {

                if (!findDumlicateFile(m_testBox, allFiles.at(i))) {

                    QListWidgetItem *newItem = new QListWidgetItem();

                    newItem->setText(allFiles.at(i));
                    m_testBox->insertItem(m_testBox->count(), newItem);
                }
            }
        }
    }
}

QString TestDbView::testFolderPath() const
{
    return m_filePath;
}

void TestDbView::setTestFolderPath(const QString &path)
{
    m_filePath = path;
    emit testFolderPathChanged(m_filePath);
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
            filename = testFolderPath() + item->text();
            QFile file(filename);

            if (file.exists()) {
                QFileInfo fileExt(filename);
                QString ext = fileExt.suffix();

                if (ext == ".doc" || ext == ".docx") {
                    emit docFileName(filename);
                } else if (SqlliteDbManager::checkIfTestDb(filename)) {
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

void TestDbView::defaulTestBoxPath(QString folderPath)
{
    if (!folderPath.isEmpty()) {

        if (folderPath.at(folderPath.count() - 1) != '/')
            folderPath = folderPath + slash;

        setTestFolderPath(folderPath);
        m_chosenFolder->setText(folderPath);
        m_testBox->clear();

        QDir entryDir(folderPath);
        if (entryDir.exists()) {

            QStringList filesList = entryDir.entryList(QDir::Files | QDir::NoDotAndDotDot);

            for (int i = 0; i < filesList.count(); i++) {

                QFileInfo fileExt(filesList.at(i));
                QString ext = fileExt.suffix().toLower();

                if (ext == ".doc" || ext == ".docx" || SqlliteDbManager::checkIfTestDb(filesList.at(i))) {
                    if (!findDumlicateFile(m_testBox, filesList.at(i))) {

                        QListWidgetItem *newItem = new QListWidgetItem();

                        newItem->setText(filesList.at(i));
                        m_testBox->insertItem(m_testBox->count(), newItem);
                    }
                }
            }
        }
    }
}
