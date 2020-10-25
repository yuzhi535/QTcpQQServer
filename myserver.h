#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QMap>
#include <QMessageBox>
#include <QByteArray>
#include "myclient.h"
#include <QDir>
#include <QLinkedList>
#include <list>
#include <QApplication>
#include <QEventLoop>
#include "mythread.h"

using std::list;

class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    MyServer();
    ~MyServer();
    void recvUserInfo(QMap<QString, QString>& tmp);
    void addUserInfo(QString& name, QString& password);
    bool isInUser(QString name);
    void parseName(QString& str, QString& name, QString& pass);
    void mySleep(qint32 sec);
    qint32 getIndex() { return ++index; }


protected:
    void incomingConnection(qintptr socketDescriptor);

signals:
    void info(QString data, QString id);
    void new_Msg(QString msg);    //to mainWindow
    void newUser(QString msg);
    void oldUser(QString user);
    void showImg(QByteArray img);

public slots:
    void sendImg(QByteArray img);
    void old_User(QString user, qint32 id);
    void newMsg(QString msg);
//    void readMsg();

private:
    QTcpServer* myServer;
    list<MyClient*> myClient;
    QMap<QString, QString> user;     //name  password
    qint32 index;
};

#endif // MYSERVER_H
