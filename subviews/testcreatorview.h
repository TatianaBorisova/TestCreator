#ifndef TESTCREATORVIEW_H
#define TESTCREATORVIEW_H

#include <QWidget>

class QGridLayout;
class QListWidget;
class QPushButton;
class QComboBox;
class QLabel;
class QStringList;
class DocFileProcessing;
class ServerSettingsView;

class TestCreatorView: public QWidget
{
    Q_OBJECT
public:
    TestCreatorView(QWidget *parent = 0);

    void resetElementsSize() const;
signals:
    void addDocument(const QString &fileName);
    void removeDocument();
    void backPressed();

private slots:
    void addDocumentInListWidget(const QString &fileName);
    void addTestToGeneratedListWidget(const QString &fileName);

    void openFileDialog();
    void generateFromDocFile();
    void removeDocumentFromListWidget();

    bool findDumlicateFile(QListWidget *itemBox, const QString &fileName);

private:
    QLabel      *m_listWidgetName;
    QLabel      *m_testTypeName;
    QGridLayout *m_centerBox;
    QListWidget *m_wordDocPreviewBox;
    QListWidget *m_generatedTestBox;
    QPushButton *m_addDocument;
    QPushButton *m_removeDocument;
    QPushButton *m_generateTest;
    QPushButton *m_backBtn;
};

#endif // TESTCREATORVIEW_H
