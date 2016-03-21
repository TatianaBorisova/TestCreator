#ifndef TESTDBVIEW_H
#define TESTDBVIEW_H

#include "testcreatorbaseview.h"

class QPushButton;
class QGridLayout;
class QListWidget;
class QVBoxLayout;
class QLabel;
class QHBoxLayout;

class TestDbView : public TestCreatorBaseView
{
    Q_OBJECT
public:
    Q_PROPERTY(QString testFolderPath READ testFolderPath WRITE setTestFolderPath NOTIFY testFolderPathChanged)

    explicit TestDbView(QWidget *parent = 0);

    void setFixedSize(int w, int h);

    void defaulTestBoxPath(QString folderPath);
    QString testFolderPath() const;
    void setTestFolderPath(const QString &path);

signals:
    void bdFileName(const QString &filename);
    void docFileName(const QString &filename);
    void testFolderPathChanged(const QString &path);

protected:
    virtual void resize();

protected slots:
    void loadFromFolder();
    void updateConsistent();
    void openExistedTest();

private:
    bool findDumlicateFile(QListWidget *itemBox, const QString &fileName);

    QGridLayout *m_box;
    QVBoxLayout *m_vbox;
    QHBoxLayout *m_hbox;
    QListWidget *m_testBox;
    QPushButton *m_openTestFile;
    QPushButton *m_createTestFile;
    QPushButton *m_folderContainer;
    QLabel      *m_header;
    QLabel      *m_chosenFolder;
    QString      m_filePath;
};

#endif // TESTDBVIEW_H
