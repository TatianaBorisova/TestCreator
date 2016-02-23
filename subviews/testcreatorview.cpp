#include "testcreatorview.h"
#include "docfileprocessing.h"

#include <QGridLayout>
#include <QComboBox>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QFont>
#include <QDebug>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QApplication>
#include <QDesktopWidget>

namespace {
const int btnWidth = 200;
}

TestCreatorView::TestCreatorView(QWidget *parent) :
    QWidget(parent),
    m_listWidgetName(new QLabel(this)),
    m_testTypeName(new QLabel(this)),
    m_centerBox(new QGridLayout(this)),
    m_wordDocPreviewBox(new QListWidget(this)),
    m_generatedTestBox(new QListWidget(this)),
    m_addDocument(new QPushButton(this)),
    m_removeDocument(new QPushButton(this)),
    m_generateTest(new QPushButton(this)),
    m_backBtn(new QPushButton(this))
{    
    connect(m_backBtn, &QPushButton::clicked, this, &TestCreatorView::backPressed);
    setLayout(m_centerBox);
}

void TestCreatorView::generateFromDocFile()
{
    if (m_wordDocPreviewBox->count() <= 0)
        return;

    DocFileProcessing doc;

    //generate test from one file
    if (m_wordDocPreviewBox->count() == 1) {
        QListWidgetItem *item = m_wordDocPreviewBox->item(0);
        if (item) {
            doc.readFromDocFile(item->text(), this);
        }
        //generate test from several files
    } else if (m_wordDocPreviewBox->count() > 1){
        for (int i = 0; i < m_wordDocPreviewBox->count(); i++) {
            QListWidgetItem *item = m_wordDocPreviewBox->item(i);
            if (item) {
                doc.readFromDocSet(item->text(), this);
            }
        }
    }

    QString generatedTest = doc.generateTestFile();
    if (!generatedTest.isEmpty()) {
        addTestToGeneratedListWidget(generatedTest);
    }

    doc.clearData();
}

void TestCreatorView::openFileDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Test Document"), "", "Doc (*.doc *.docx)");
    emit addDocument(fileName);
}

bool TestCreatorView::findDumlicateFile(QListWidget *itemBox, const QString &fileName)
{
    if (itemBox) {
        for (int i = 0; i < itemBox->count(); i++) {
            if (fileName == itemBox->item(i)->text())
                return true;
        }
    }
    return false;
}

void TestCreatorView::addDocumentInListWidget(const QString &fileName)
{
    if (!findDumlicateFile(m_wordDocPreviewBox, fileName)) {

        QListWidgetItem *newItem = new QListWidgetItem();
        newItem->setText(fileName);

        m_wordDocPreviewBox->insertItem(m_wordDocPreviewBox->count(), newItem);
        m_wordDocPreviewBox->setCurrentItem(newItem);
    } else {
        QMessageBox::warning(0, tr("Dublicate file"), tr("Файл уже выбран."));
        return;
    }

    if (m_wordDocPreviewBox->count() <= 1)
        m_generateTest->setText("Сгенерировать тест из файла");
    else
        m_generateTest->setText("Сгенерировать тест из нескольких файлов");
}

void TestCreatorView::addTestToGeneratedListWidget(const QString &fileName)
{
    if (!findDumlicateFile(m_generatedTestBox, fileName)) {

        QListWidgetItem *newItem = new QListWidgetItem();
        newItem->setText(fileName);

        m_generatedTestBox->insertItem(m_generatedTestBox->count(), newItem);
        m_generatedTestBox->setCurrentItem(newItem);
    } else {
        QMessageBox::warning(0, tr("Dublicate file"), tr("Файл уже выбран."));
    }
}

void TestCreatorView::removeDocumentFromListWidget()
{
    m_wordDocPreviewBox->takeItem(m_wordDocPreviewBox->row(m_wordDocPreviewBox->currentItem()));

    if (m_wordDocPreviewBox->count() <= 1)
        m_generateTest->setText("Сгенерировать тест из файла");
    else
        m_generateTest->setText("Сгенерировать тест из нескольких файлов");
}

void TestCreatorView::resetElementsSize() const
{
    QFont buttonsFont;
    buttonsFont.setPixelSize(14);

    m_wordDocPreviewBox->setFixedSize(this->width()*0.45, this->height()/2);
    m_generatedTestBox->setFixedSize(this->width()*0.45, this->height()/2);
    m_listWidgetName->setFixedSize(this->width()*0.45, 50);
    m_testTypeName->setFixedSize(this->width()*0.38, 50);
    m_addDocument->setFixedSize(btnWidth, 50);
    m_removeDocument->setFixedSize(btnWidth, 50);
    m_generateTest->setFixedSize(btnWidth, 50);
    m_backBtn->setFixedSize(50, 50);

    m_wordDocPreviewBox->setFont(buttonsFont);
    m_generatedTestBox->setFont(buttonsFont);

    QFont listHeaderFont;
    listHeaderFont.setBold(true);
    listHeaderFont.setPixelSize(20);

    m_listWidgetName->setFont(listHeaderFont);
    m_listWidgetName->setText("Выбранные документы:");

    m_testTypeName->setFont(listHeaderFont);
    m_testTypeName->setText("Сгенерированный тест:");

    m_removeDocument->setFont(buttonsFont);
    m_addDocument->setFont(buttonsFont);
    m_generateTest->setFont(buttonsFont);
    m_backBtn->setFont(buttonsFont);

    m_removeDocument->setText("Удалить тест-документ");
    m_addDocument->setText("Добавить тест-документ");
    m_generateTest->setText("Сгенерировать тест из файла");

    m_backBtn->setIcon(QIcon(QPixmap(":res/back.png")));
    m_backBtn->setIconSize(QSize(50, 50));

    connect(m_addDocument, SIGNAL(clicked(bool)), this, SLOT(openFileDialog()));
    connect(m_removeDocument, SIGNAL(clicked(bool)), this, SIGNAL(removeDocument()));
    connect(m_generateTest, SIGNAL(clicked(bool)), this, SLOT(generateFromDocFile()));

    connect(this, SIGNAL(addDocument(const QString &)), this, SLOT(addDocumentInListWidget(const QString &)));
    connect(this, SIGNAL(removeDocument()), this, SLOT(removeDocumentFromListWidget()));

    m_centerBox->addWidget(m_listWidgetName, 1, 0);
    m_centerBox->addWidget(m_testTypeName, 1, 1);

    m_centerBox->addWidget(m_wordDocPreviewBox, 2, 0);
    m_centerBox->addWidget(m_generatedTestBox, 2, 1);

    m_centerBox->addWidget(m_addDocument, 3, 0);
    m_centerBox->addWidget(m_generateTest, 3, 1);

    m_centerBox->addWidget(m_removeDocument, 4, 0);

    m_centerBox->addWidget(m_backBtn, 0, 1);
    m_centerBox->setAlignment(m_backBtn, Qt::AlignRight);


    int margin = (width() - 2*m_wordDocPreviewBox->width() - m_centerBox->spacing())/2;
    m_centerBox->setContentsMargins(20, 20, 20, margin);
    m_centerBox->setGeometry(this->rect());
}

