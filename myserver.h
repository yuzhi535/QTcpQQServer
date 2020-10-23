#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QList>
#include <QMessageBox>
#include <QByteArray>
#include "myclient.h"
#include <QDir>
#include "mythread.h"

class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    MyServer();
    ~MyServer();

protected:
    void incomingConnection(qintptr socketDescriptor);

signals:
    void info(QString data, QString id);
    void addItem(QString name);
    void addFile(QByteArray& file, QString id);

signals:
    void newUser(QString id);     //新用户
    void deleteUser(QString id);  //断开连接
    void newMsg(QString msg);
    void newFile(QString file);   //数据类型有待考虑


public slots:
    void addInfo(QString data, QString id);
    void closeClient();
    void sendFile(QByteArray& file, QString id);

private:
    QTcpServer* myServer;
    QMap<QString, MyClient*> client;
};

#endif // MYSERVER_H
