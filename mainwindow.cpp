#include "mainwindow.h"
#include "docfileprocessing.h"

///////////////////////////////
#include "serverview.h"

#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QFont>
#include <QDebug>
#include <QMessageBox>
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    m_listWidgetName(new QLabel(this)),
    m_testTypeName(new QLabel(this)),
    m_leftBox(new QVBoxLayout(this)),
    m_rightBox(new QVBoxLayout(this)),
    m_centerBox(new QHBoxLayout(this)),
    m_wordDocPreviewBox(new QListWidget(this)),
    m_generatedTestBox(new QListWidget(this)),
    m_addDocument(new QPushButton(this)),
    m_removeDocument(new QPushButton(this)),
    m_generateTest(new QPushButton(this)),
    m_serverView(new ServerView(this))
{
    setFixedSize(1920, 1080);

    QFont buttonsFont;
    buttonsFont.setPixelSize(14);

    m_wordDocPreviewBox->setFixedSize(this->width()*0.45, this->height()/2);
    m_generatedTestBox->setFixedSize(this->width()*0.45, this->height()/2);
    m_listWidgetName->setFixedSize(this->width()*0.45, 50);
    m_testTypeName->setFixedSize(500, 50);
    m_addDocument->setFixedSize(300, 50);
    m_removeDocument->setFixedSize(300, 50);
    m_generateTest->setFixedSize(300, 50);

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

    m_removeDocument->setText("Удалить тест-документ");
    m_addDocument->setText("Добавить тест-документ");
    m_generateTest->setText("Сгенерировать тест из файла");

    connect(m_addDocument, SIGNAL(clicked(bool)), this, SLOT(openFileDialog()));
    connect(m_removeDocument, SIGNAL(clicked(bool)), this, SIGNAL(removeDocument()));
    connect(m_generateTest, SIGNAL(clicked(bool)), this, SLOT(generateFromDocFile()));

    connect(this, SIGNAL(addDocument(const QString &)), this, SLOT(addDocumentInListWidget(const QString &)));
    connect(this, SIGNAL(removeDocument()), this, SLOT(removeDocumentFromListWidget()));

    m_leftBox->addWidget(m_listWidgetName);
    m_leftBox->addWidget(m_wordDocPreviewBox);
    m_leftBox->addWidget(m_addDocument);
    m_leftBox->addWidget(m_removeDocument);
    m_leftBox->addWidget(m_serverView);

    m_leftBox->addSpacing((this->height()
                           - m_listWidgetName->height()
                           - m_wordDocPreviewBox->height()
                           - m_addDocument->height()
                           - m_removeDocument->height()
                           - m_serverView->height())*0.35);

    m_rightBox->addWidget(m_testTypeName);
    m_rightBox->addWidget(m_generatedTestBox);
    m_rightBox->addWidget(m_generateTest);

    m_rightBox->addSpacing((this->height()
                            - m_testTypeName->height()
                            - m_generateTest->height()
                            - m_generatedTestBox->height())*0.8);

    m_centerBox->addLayout(m_leftBox);
    m_centerBox->addSpacing(m_wordDocPreviewBox->width() + m_generatedTestBox->width() + 40);
    m_centerBox->setMargin(20);
    m_centerBox->addLayout(m_rightBox);

    m_centerBox->setGeometry(this->rect());

    setLayout(m_centerBox);
}

void MainWindow::generateFromDocFile()
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

void MainWindow::openFileDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Test Document"), "", "Doc (*.doc *.docx)");
    emit addDocument(fileName);
}

bool MainWindow::findDumlicateFile(QListWidget *itemBox, const QString &fileName)
{
    if (itemBox) {
        for (int i = 0; i < itemBox->count(); i++) {
            if (fileName == itemBox->item(i)->text())
                return true;
        }
    }
    return false;
}

void MainWindow::addDocumentInListWidget(const QString &fileName)
{
    if (!findDumlicateFile(m_wordDocPreviewBox, fileName)) {

        QListWidgetItem *newItem = new QListWidgetItem();
        newItem->setText(fileName);

        m_wordDocPreviewBox->insertItem(m_wordDocPreviewBox->count(), newItem);
    } else {
        QMessageBox::warning(0, tr("Dublicate file"), tr("Файл уже выбран."));
    }

    if (m_wordDocPreviewBox->count() <= 1)
        m_generateTest->setText("Сгенерировать тест из файла");
    else
        m_generateTest->setText("Сгенерировать тест из нескольких файлов");
}

void MainWindow::addTestToGeneratedListWidget(const QString &fileName)
{
    if (!findDumlicateFile(m_generatedTestBox, fileName)) {

        QListWidgetItem *newItem = new QListWidgetItem();
        newItem->setText(fileName);

        m_generatedTestBox->insertItem(m_generatedTestBox->count(), newItem);
    } else {
        QMessageBox::warning(0, tr("Dublicate file"), tr("Файл уже выбран."));
    }
}

void MainWindow::removeDocumentFromListWidget()
{
    m_wordDocPreviewBox->takeItem(m_wordDocPreviewBox->row(m_wordDocPreviewBox->currentItem()));

    if (m_wordDocPreviewBox->count() <= 1)
        m_generateTest->setText("Сгенерировать тест из файла");
    else
        m_generateTest->setText("Сгенерировать тест из нескольких файлов");
}
