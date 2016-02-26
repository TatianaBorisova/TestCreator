#ifndef TESTEDITORSUBVIEW_H
#define TESTEDITORSUBVIEW_H

#include "views/testcreatorbaseview.h"

class QLabel;
class QLineEdit;
class QPushButton;
class QGridLayout;
class QTimeEdit;

class TestEditorSubView : public TestCreatorBaseView
{
    Q_OBJECT
public:
    explicit TestEditorSubView(QWidget *parent = 0);
    void setFixedSize(int w, int h);

    void setTestName();
    void setTestTime();
    void setQuestionCount();

signals:
    void testNameChanged(const QString &name);
    void testTimeChanged(const QTime &time);
    void questionCountChanged(int value);
    void saveDataInDb();
    void loadedDocFile(const QString &filename);
    void questionCountLoadedFromFile(int value);

public slots:
    void loadTestFileData(const TestData &data);

protected:
    virtual void resize();

private slots:
    void addQuestions();
    void loadDocFile();
    void showQuestionsListView();

private:
    QGridLayout *m_box;
    QLabel *m_testName;
    QLabel *m_testTime;
    QLabel *m_questionCount;
    QLineEdit *m_testNameBox;
    QTimeEdit *m_testTimeBox;
    QLineEdit *m_questionCountBox;
    QPushButton *m_addQuestions;
    QPushButton *m_loadFromDoc;
    QPushButton *m_saveinDb;
    QPushButton *m_questionView;
};

#endif // TESTEDITORSUBVIEW_H
