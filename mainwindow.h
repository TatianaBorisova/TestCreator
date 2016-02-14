#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QVBoxLayout;
class QHBoxLayout;
class QListWidget;
class QPushButton;
class QComboBox;
class QLabel;
class QStringList;
class DocFileProcessing;
class ServerView;

class MainWindow: public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

signals:
    void addDocument(const QString &fileName);
    void removeDocument();

private slots:
    void openFileDialog();
    void addDocumentInListWidget(const QString &fileName);
    void addTestToGeneratedListWidget(const QString &fileName);
    void generateFromDocFile();
    void removeDocumentFromListWidget();
    bool findDumlicateFile(QListWidget *itemBox, const QString &fileName);

private:
    QLabel      *m_listWidgetName;
    QLabel      *m_testTypeName;
    QVBoxLayout *m_leftBox;
    QVBoxLayout *m_rightBox;
    QHBoxLayout *m_centerBox;
    QListWidget *m_wordDocPreviewBox;
    QListWidget *m_generatedTestBox;
    QPushButton *m_addDocument;
    QPushButton *m_removeDocument;
    QPushButton *m_generateTest;

    /////////////////////
    ServerView *m_serverView;
};

#endif // MAILWINDOW_H
