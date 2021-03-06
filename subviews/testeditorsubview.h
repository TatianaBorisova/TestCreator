#ifndef TESTEDITORSUBVIEW_H
#define TESTEDITORSUBVIEW_H

#include "views/testcreatorbaseview.h"

class QLabel;
class QLineEdit;
class QPushButton;
class QGridLayout;
class QTimeEdit;
class QTextEdit;
class QComboBox;

class TestEditorSubView : public TestCreatorBaseView
{
    Q_OBJECT
public:
    explicit TestEditorSubView(QWidget *parent = 0);
    void setFixedSize(int w, int h);

signals:
    void testNameChanged(const QString &name);
    void testTimeChanged(const QTime &time);
    void testTypeChanged(int type);
    void questionCountChanged(int value);
    void saveDataInDb();
    void loadedDocFile(const QString &filename);
    void loadedDBFile(const QString &filename);
    void questionCountLoadedFromFile(int value);

public slots:
    void loadTestFileData(const TestData &data);

protected:
    virtual void resize();

private slots:
    void setTestName();
    void setTestTime();
    void setTestType(int index);
    void setQuestionCount();

    void addQuestions();
    void loadDocFile();
    void loadDBFile();
    void showQuestionsListView();

private:
    QGridLayout *m_box;
    QLabel *m_testName;
    QLabel *m_testTime;
    QLabel *m_testType;
    QLabel *m_questionCount;
    QTextEdit *m_testNameBox;
    QTimeEdit *m_testTimeBox;
    QComboBox *m_testTypeBox;
    QLineEdit *m_questionCountBox;
    QPushButton *m_addQuestions;
    QPushButton *m_loadFromDoc;
    QPushButton *m_loadFromDb;
    QPushButton *m_saveinDb;
    QPushButton *m_questionView;
};

#endif // TESTEDITORSUBVIEW_H
