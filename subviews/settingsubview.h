#ifndef SETTINGSUBVIEW_H
#define SETTINGSUBVIEW_H

#include "views/testcreatorbaseview.h"

class QHBoxLayout;
class QGridLayout;
class TcpServer;
class QPushButton;
class QLabel;
class QLineEdit;
class QPlainTextEdit;

class SettingSubView : public TestCreatorBaseView
{
    Q_OBJECT
public:
    Q_PROPERTY(QString testDb READ testDb NOTIFY testDbChanged)
    Q_PROPERTY(QString resultDb READ resultDb NOTIFY resultDbChanged)

    explicit SettingSubView(QWidget *parent = 0);

    void startServer();
    void setFixedSize(int w, int h);

    QString testDb() const;
    QString resultDb() const;

signals:
    void testDbChanged(const QString &value);
    void resultDbChanged(const QString &value);

protected:
    virtual void resize();

private slots:
    void chooseTestDB();
    void chooseResDB();
    void setStartSetverState();
    void setStopSetverState();

private:
    QGridLayout *m_grid;
    TcpServer   *m_server;
    QLabel      *m_localhost;
    QLineEdit   *m_port;
    QLabel      *m_serverStatus;
    QPlainTextEdit   *m_testBox;
    QPlainTextEdit   *m_resultBox;
    QPushButton *m_startBtn;
    QPushButton *m_stopBtn;
    QPushButton *m_chooseTestDb;
    QPushButton *m_chooseResDb;
    QHBoxLayout *m_rowBox;
};

#endif // SETTINGSUBVIEW_H
