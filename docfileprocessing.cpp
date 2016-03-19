#include "docfileprocessing.h"
#include "stringencryption.h"

#include <QStringList>
#include <QDebug>
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QMessageBox>

namespace {
const QString header = QString("название:");
const QString doingTime = QString("время выполнения:");
const QString questionCount = QString("количество вопросов:");
const QString correctAnswer = QString("верный ответ:");
const QString incorrectAnswer = QString("неверные ответы:");
const QString picturePath = QString("картинка:");
const QString statement = QString("утверждение");
QString jsonString = QString("\t\t{\n\t\t\t\"TestQuestion\": \"%1\",\n\t\t\t\"CorrectTestAnswer\": \"%2\",\n\t\t\t\"TestAnswers\": \"%3\",\n\t\t\t\"ImgPath\": \"%4\"\n\t\t}");
const QString startFileBrace = QString("{\n");
const QString openBrace = QString("\t\t{\n");
const QString closeBrace = QString("\n\t]\n}\n");
const QString arrayDeclare = QString("\t\"testcontentarray\": [\n");
const QString comma = QString(",\n");
}

DocFileProcessing::DocFileProcessing(QObject *parent) :
    QObject(parent),
    m_wordApp(NULL)
{
}

void DocFileProcessing::readFromDocSet(const QString &filename, QWidget *parent)
{
    if (!filename.isEmpty()) {
        if (m_wordApp)
            delete m_wordApp;

        setSavingFileName(filename);
        m_docTextString.clear();
        m_testList.clear();

        m_wordApp = new QAxWidget("Word.Application", parent);
        m_wordApp->setGeometry(parent->rect());
        m_wordApp->setControl(filename);

        QAxObject *words = m_wordApp->querySubObject("Words");
        int countWord = words->dynamicCall("Count()").toInt();

        for (int a = 1; a <= countWord; a++){
            m_docTextString += QString(words->querySubObject("Item(int)", a)->dynamicCall("Text()").toString()).toLower();
        }

        QString statementStr("");
        while(!(statementStr = getStatementString()).isEmpty()) {
            m_testList.append(statementStr);
        }

        for (int i = 0; i < m_testList.count(); i++) {
            fillTestQuestionInfo(m_testList.at(i));
        }
    }
}

TestData DocFileProcessing::readFromDocFile(const QString &filename, QWidget *parent)
{
    m_loadedData.questions.clear();
    m_loadedData.questionCount = 0;
    m_loadedData.testName.clear();
    m_loadedData.testTime = QTime(0, 0);

    if (!filename.isEmpty()) {

        setSavingFileName(filename);
        clearData();

        m_wordApp = new QAxWidget("Word.Application", parent);
        m_wordApp->setGeometry(parent->rect());
        m_wordApp->setControl(filename);

        QAxObject *words = m_wordApp->querySubObject("Words");
        int countWord = words->dynamicCall("Count()").toInt();

        for (int a = 1; a <= countWord; a++){
            m_docTextString += QString(words->querySubObject("Item(int)", a)->dynamicCall("Text()").toString()).toLower();
        }

        takeTestHeaderInfo();

        QString statementStr("");
        while(!(statementStr = getStatementString()).isEmpty()) {
            m_testList.append(statementStr);
        }

        for (int i = 0; i < m_testList.count(); i++) {
            fillTestQuestionInfo(m_testList.at(i));
        }

        for (int i = 0; i < m_statementList.count(); i++) {
            TestQuestions question;
            question.question = StringEncryption::stringEncrypt(addUpperSymbol(m_statementList.at(i)), encryptKey);
            question.weight   = 1;
            question.answers.correctAnswer    = StringEncryption::stringEncrypt(m_answerList.at(i).correctAnswer, encryptKey);
            question.answers.uncorrectAnswers = StringEncryption::stringEncrypt(m_answerList.at(i).uncorrectAnswers, encryptKey);
            question.answers.imgName          = m_answerList.at(i).imgName;

            m_loadedData.questions.append(question);
        }
    }
    return m_loadedData;
}

QString DocFileProcessing::generateJsonTestFile() const
{
    qDebug() << "generateTestFile";

    QDir dir = QDir::current();
    dir.mkdir(m_testFileName);
    //    dir.cd(m_testFileName);

    QString testSubDir = m_testFileName + "/";
    //TBD dont forget about linux path
    QFile file(testSubDir + m_testFileName + ".json");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return "";
    }

    QTextStream out(&file);
    out << startFileBrace;
    out << arrayDeclare;

    for (int i = 0; i < m_statementList.count(); i++) {
        //if last one
        if (i == m_statementList.count() - 1)
            out << jsonString.arg(StringEncryption::stringEncrypt(m_statementList.at(i), encryptKey))
                   .arg(StringEncryption::stringEncrypt(m_answerList.at(i).correctAnswer, encryptKey))
                   .arg(StringEncryption::stringEncrypt(m_answerList.at(i).uncorrectAnswers, encryptKey))
                   .arg(StringEncryption::stringEncrypt(m_answerList.at(i).imgName, encryptKey));
        else
            out << jsonString.arg(StringEncryption::stringEncrypt(m_statementList.at(i), encryptKey))
                   .arg(StringEncryption::stringEncrypt(m_answerList.at(i).correctAnswer, encryptKey))
                   .arg(StringEncryption::stringEncrypt(m_answerList.at(i).uncorrectAnswers, encryptKey))
                   .arg(StringEncryption::stringEncrypt(m_answerList.at(i).imgName, encryptKey)) << comma;

        //if img exists lets copy it to test directory
        if (!m_answerList.at(i).imgName.isEmpty()) {

            QString imgPath = m_answerList.at(i).imgName;
            imgPath = imgPath.replace(" ", "");

            if (!QFile::copy(QDir::currentPath() + "/" + imgPath, testSubDir + imgPath))
                QMessageBox::warning(0, "Error", "Ошибка. Не найден файл:\n" + imgPath);
        }
    }

    out << closeBrace;
    file.close();

    return QDir::current().absoluteFilePath(file.fileName());
}

void DocFileProcessing::fillTestQuestionInfo(QString str)
{
    int correctAnswerIndx = str.indexOf(correctAnswer);
    int incorrectAnswerIndx = str.indexOf(incorrectAnswer);
    int imgPathIndx = str.indexOf(picturePath);

    if (correctAnswerIndx < 0) {
        QMessageBox::warning(0, "Error", "Ошибка. Не найден верный вариант\nответа в блоке:\n" + str);
        return;
    }

    if (incorrectAnswerIndx < 0) {
        QMessageBox::warning(0, "Error", "Ошибка. Не найдены неверные варианты\nответов в блоке:\n" + str);
        return;
    }

    if (correctAnswerIndx > incorrectAnswerIndx) {
        QMessageBox::warning(0, "Error", "Ошибка. Неверный порядок утверждений в блоке:\n"
                             + str
                             + "\nПравильный вид:\nУтверждение:...\nВерный ответ:...\nНеверные ответы:...\nКартинка:...\n");
        return;
    }

    QString statement = str.left(correctAnswerIndx);
    statement = clearStatementString(statement);

    m_statementList.append(statement);

    QString correct, uncorrect, imgstring;

    //////////////////first chatacter/////////////////////////////length/////////////////////////////////////////////////////////////
    correct = str.mid(correctAnswerIndx + correctAnswer.length(), incorrectAnswerIndx - (correctAnswerIndx + correctAnswer.length()));

    //only if image path exists
    if (imgPathIndx > 0) {
        uncorrect = str.mid(incorrectAnswerIndx + incorrectAnswer.length(), imgPathIndx - (incorrectAnswerIndx + incorrectAnswer.length()));
        imgstring = str.right(str.length() - (imgPathIndx + picturePath.length()));
    } else { //overwise
        uncorrect = str.right(str.length() - (incorrectAnswerIndx + incorrectAnswer.length()));
    }

    correct   = clearStatementString(correct);
    uncorrect = clearStatementString(uncorrect);
    imgstring = clearStatementString(imgstring);

    Answers answer;
    answer.correctAnswer    = correct;
    answer.uncorrectAnswers = uncorrect;
    answer.imgName          = imgstring;

    m_answerList.append(answer);
}

void DocFileProcessing::clearData()
{
    if (m_wordApp)
        delete m_wordApp;

    m_testList.clear();
    m_statementList.clear();
    m_answerList.clear();
    m_docTextString.clear();
}

/////////////////////finish loading from file
QString DocFileProcessing::getTestNameString(const QString &filetext)
{
    QString result("");
    QString text = filetext;

    int startInd = text.lastIndexOf(header);
    int endInd = text.lastIndexOf(doingTime);

    if (startInd != -1) {
        result = text.mid(startInd + header.count(), endInd - (startInd + header.count()));
    }

    return clearStatementString(result);
}

QString DocFileProcessing::getTestTimeString(const QString &filetext)
{
    QString result("");
    QString text = filetext;

    int startInd = text.indexOf(doingTime);
    int endInd = text.indexOf(questionCount);

    if (startInd != -1) {
        result = text.mid(startInd + doingTime.count(), endInd - (startInd + doingTime.count()));
    }

    return clearStatementString(result);
}

QString DocFileProcessing::getQuestCountString(const QString &filetext)
{
    QString result("");
    QString text = filetext;

    int startInd = text.indexOf(questionCount);
    int endInd = text.indexOf(statement);

    if (startInd != -1) {
        result = text.mid(startInd + questionCount.count(), endInd - (startInd + questionCount.count()));
    }

    return clearStatementString(result);
}

QString DocFileProcessing::getStatementString()
{
    QString result("");
    int firstIndex = -1;

    if ((firstIndex = m_docTextString.indexOf(statement + " : ")) >= 0) {
        result = firstTestString(firstIndex, statement + " : ");
    } else if ((firstIndex = m_docTextString.indexOf(statement + " :")) >= 0) {
        result = firstTestString(firstIndex, statement + " :");
    } else if ((firstIndex = m_docTextString.indexOf(statement + ": ")) >= 0) {
        result = firstTestString(firstIndex, statement + ": ");
    } else if ((firstIndex = m_docTextString.indexOf(statement + ":")) >= 0) {
        result = firstTestString(firstIndex, statement + ":");
    } else if ((firstIndex = m_docTextString.indexOf(statement)) >= 0) {
        result = firstTestString(firstIndex, statement);
    }
    return result;
}

QString DocFileProcessing::firstTestString(int firstIndex, QString readStatement)
{
    QString result("");

    m_docTextString = m_docTextString.remove(firstIndex, readStatement.size());

    int secondIndex = m_docTextString.indexOf(readStatement);
    if (secondIndex >= 0) {
        result = m_docTextString.left(secondIndex);
        m_docTextString = m_docTextString.remove(result);
    } else {
        result = m_docTextString;
    }

    return result;
}

QString DocFileProcessing::clearAnswerString(const QString &str)
{
    QRegExp regExp("[:\",.;. / \r\n]");
    QString result = str;

    return result.remove(regExp);
}

QString DocFileProcessing::clearStatementString(const QString &str)
{
    QRegExp regExp("[\r\n]");
    QString result = str;

    result = result.replace(regExp, ""); //change regexp to empty character

    while(result.indexOf(" ") == 0)
        result = result.remove(0, 1);

    while(result.count() && result.lastIndexOf(" ") == result.count() - 1) {
        result = result.remove(result.count() - 1, 1);
    }

    return result;
}

void DocFileProcessing::printReadData()
{
    for (int i = 0; i < m_statementList.count(); i++) {
        qDebug() << "readFromDocSet statement: " << m_statementList.at(i);
        qDebug() << "readFromDocSet answers: " << m_answerList.at(i).correctAnswer << m_answerList.at(i).uncorrectAnswers;
    }
}

void DocFileProcessing::setSavingFileName(const QString &file)
{
    m_testFileName.clear();
    m_testFileName = getFileName(file);
}

QString DocFileProcessing::getFileName(const QString &file, bool withFileExtention) const
{
    QString fillName("");
    if (!file.isEmpty()) {

        //TBD: dont forget about linux file path
        int lastSlashIndx = file.lastIndexOf("/");
        int lastPointIndx = file.lastIndexOf(".");

        QFile realfile(file);
        if (realfile.exists()) {
            if (!withFileExtention)
                fillName = file.mid(lastSlashIndx + 1, lastPointIndx - (lastSlashIndx + 1));
            else
                fillName = file.right(file.count() - lastSlashIndx);
        }
    }

    return fillName;
}

void DocFileProcessing::takeTestHeaderInfo()
{
    m_loadedData.testName = addUpperSymbol(getTestNameString(m_docTextString));
    m_loadedData.testTime = QTime::fromString(getTestTimeString(m_docTextString), "hh:mm");
    m_loadedData.questionCount = getQuestCountString(m_docTextString).toInt();

    int testHeaderStart = m_docTextString.indexOf(header);
    int testHeaderEnd = m_docTextString.indexOf(statement);

    m_docTextString = m_docTextString.remove(testHeaderStart, testHeaderEnd);
}

QString DocFileProcessing::addUpperSymbol(const QString &str)
{
    QString firstSymbol("");
    QString result = str;

    if (!result.isEmpty()) {
        firstSymbol = result.at(0).toUpper();
        result = result.remove(0, 1);
        result = firstSymbol + result;
    }
    return result;
}
