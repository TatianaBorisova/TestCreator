#ifndef TESTDBVIEW_H
#define TESTDBVIEW_H

#include "testcreatorbaseview.h"

class QPushButton;
class QGridLayout;
class QListWidget;
class QVBoxLayout;

class TestDbView : public TestCreatorBaseView
{
    Q_OBJECT
public:
    explicit TestDbView(QWidget *parent = 0);

    void setFixedSize(int w, int h);

signals:
    void bdFileName(const QString &filename);
    void docFileName(const QString &filename);


protected:
    virtual void resize();

protected slots:
    void loadFromFolder();
    void createNewTest();
    void openExistedTest();

private:
    bool findDumlicateFile(QListWidget *itemBox, const QString &fileName);
    void fillChoiceBox(QString folderPath);
    bool checkIfSqlliteDb(const QString &filename);

    QGridLayout *m_box;
    QVBoxLayout *m_vbox;
    QListWidget *m_testBox;
    QPushButton *m_openTestFile;
    QPushButton *m_createTestFile;
    QPushButton   *m_folderContainer;
};

#endif // TESTDBVIEW_H
