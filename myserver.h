#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QMap>
#include <QMessageBox>
#include <QByteArray>
#include "myclient.h"
#include <QDir>
#include <QList>
#include "mythread.h"

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


protected:
    void incomingConnection(qintptr socketDescriptor);

signals:
    void info(QString data, QString id);
    void new_Msg(QString msg);    //to mainWindow
    void newUser(QString msg);
    void oldUser(QString user);

public slots:
    void closeClient();
    void sendFile(QByteArray& file, QString id);
    void old_User(QString user, qint32 id);
    void newMsg(QString msg);


private:
    QTcpServer* myServer;
    QList<MyClient*> myClient;
    QVector<myThread*> threadPool;
    QMap<QString, QString> user;     //name  password
    QString tmp;    //临时存储用户名
};

#endif // MYSERVER_H
