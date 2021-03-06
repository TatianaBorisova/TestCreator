#include "testeditorsubview.h"

#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QRegExpValidator>
#include <QTimeEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextEdit>
#include <QComboBox>
#include <QDebug>

TestEditorSubView::TestEditorSubView(QWidget *parent) :
    TestCreatorBaseView(parent),
    m_box(new QGridLayout(this)),
    m_testName(new QLabel("Название теста", this)),
    m_testTime(new QLabel("Время выполнения теста", this)),
    m_testType(new QLabel("Тип теста", this)),
    m_questionCount(new QLabel("Количество вопросов", this)),
    m_testNameBox(new QTextEdit(this)),
    m_testTimeBox(new QTimeEdit(this)),
    m_testTypeBox(new QComboBox(this)),
    m_questionCountBox(new QLineEdit(this)),
    m_addQuestions(new QPushButton("Редактировать вопросы", this)),
    m_loadFromDoc(new QPushButton("Загрузить из Doc", this)),
    m_loadFromDb(new QPushButton("Загрузить из БД", this)),
    m_saveinDb(new QPushButton("Сохранить в БД", this)),
    m_questionView(new QPushButton("Просмотреть вопросы", this))
{
    connect(m_addQuestions, &QPushButton::clicked,   this, &TestEditorSubView::addQuestions);
    connect(m_testNameBox,  &QTextEdit::textChanged, this, &TestEditorSubView::setTestName);
    connect(m_testTimeBox,  &QTimeEdit::editingFinished,     this, &TestEditorSubView::setTestTime);
    connect(m_testTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setTestType(int)));
    connect(m_questionCountBox, &QLineEdit::editingFinished, this, &TestEditorSubView::setQuestionCount);
    connect(m_saveinDb,         &QPushButton::clicked, this, &TestEditorSubView::saveDataInDb);
    connect(m_loadFromDoc,  &QPushButton::clicked, this, &TestEditorSubView::loadDocFile);
    connect(m_loadFromDb,   &QPushButton::clicked, this, &TestEditorSubView::loadDBFile);
    connect(m_questionView, &QPushButton::clicked, this, &TestEditorSubView::showQuestionsListView);

    m_testNameBox->setFixedHeight(50);
    m_questionCountBox->setValidator(new QRegExpValidator(QRegExp("\\d+"), this));
    m_box->setSpacing(20);

    m_testTypeBox->addItem("Тест \"Верно ли утверждение\"", 0);
    m_testTypeBox->addItem("Тест \"Выбор правильного варианта\"", 1);

    m_box->addWidget(m_testName,      0, 0);
    m_box->addWidget(m_testTime,      1, 0);
    m_box->addWidget(m_testType,      2, 0);
    m_box->addWidget(m_questionCount, 3, 0);
    m_box->addWidget(m_loadFromDoc,   0, 2);
    m_box->addWidget(m_loadFromDb,    0, 3);
    m_box->addWidget(m_saveinDb,      4, 0);

    m_box->addWidget(m_testNameBox,      0, 1);
    m_box->addWidget(m_testTimeBox,      1, 1);
    m_box->addWidget(m_testTypeBox,      2, 1);
    m_box->addWidget(m_questionCountBox, 3, 1);
    m_box->addWidget(m_addQuestions,     3, 2);
    m_box->addWidget(m_questionView,     3, 3);

    m_box->setMargin(30);
    m_box->setSpacing(30);

    setLayout(m_box);
}

void TestEditorSubView::loadTestFileData(const TestData &data)
{
    m_testNameBox->setText(data.testName);
    m_testTimeBox->setTime(data.testTime);
    m_questionCountBox->setText(QString::number(data.questionCount));
    m_testTypeBox->setCurrentIndex(data.testType);
    emit questionCountLoadedFromFile(m_questionCountBox->text().toInt());
}

void TestEditorSubView::resize()
{
}

void TestEditorSubView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    resize();
}

void TestEditorSubView::addQuestions()
{
    if (m_questionCountBox->text().toInt() > 0)
        emit showSubView(QuestionEditor);
    else
        QMessageBox::warning(0, "Внимание", "Сперва введите количество вопросов.");
}

void TestEditorSubView::loadDocFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Choose Test Document"), "", "DOC(*.doc *.docx)");
    emit loadedDocFile(filePath);
}

void TestEditorSubView::loadDBFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Choose Test Document"), "");
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        file.seek(0);
        QByteArray bytes = file.read(16);
        if (QString(bytes.data()).contains("SQLite format"))
            emit loadedDBFile(filePath);
        else
            QMessageBox::warning(0, "Ошибка", "Выбранный файл не файл SQLite базы данных.");
    }
}

void TestEditorSubView::setTestName()
{
    emit testNameChanged(m_testNameBox->toPlainText());
}

void TestEditorSubView::setTestTime()
{
    emit testTimeChanged(m_testTimeBox->time());
}

void TestEditorSubView::setQuestionCount()
{
    emit questionCountChanged(m_questionCountBox->text().toInt());
}

void TestEditorSubView::showQuestionsListView()
{
    emit showSubView(QuestionViewer);
}

void TestEditorSubView::setTestType(int index)
{
    emit testTypeChanged(index);
}
