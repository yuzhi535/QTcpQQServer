#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QGridLayout>
#include <QFile>
#include <QDir>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>


#include "myserver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:


public slots:
    void on_button_1_clicked();
    void newUser(QString name);
    void newMsg(QString msg);
    void old_user(QString user);

private:
    Ui::MainWindow *ui;
    QPushButton button_1;     //连接
    QPushButton button_2;     //退出
    QLabel label_1;           //ip
    QLabel label_2;           //port
    QLineEdit line_1;         //ip
    QLineEdit line_2;         //port
    QListWidget text;         //show the info
    QListWidget sysLog;       //show the connection info
    QWidget contral;          //contral widget
    QGridLayout m_layout;     //layout

    MyServer* server;
};
#endif // MAINWINDOW_H
