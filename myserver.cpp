#include "myserver.h"
#include <QString>

MyServer::MyServer()
{
    myServer = new QTcpServer();
}

MyServer::~MyServer()
{

}

bool MyServer::isInUser(QString name)
{
    if (user.contains(name))
        return false;
    return true;
}

void MyServer::parseName(QString& str, QString& name, QString& pass)
{
    bool flag = true;
    int j = 1;
    for (int i = j; i < str.size(); ++i)
    {
        if (str[i] == '\b' && flag)
        {
            flag = false;
            name = str.mid(j, i - j);
            j = i + 1;
        }
        else if (str[i] == '\b')
            pass = str.mid(j, i - j);
    }
}

void MyServer::mySleep(qint32 sec)
{
    QTime time1 = QTime::currentTime().addMSecs(sec);
    while (QTime::currentTime() < time1) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void MyServer::incomingConnection(qintptr socketDescriptor)
{
    MyClient* client = new MyClient();
    client->setSocketDescriptor(socketDescriptor);

    QString name, pass, login;
    connect(client, &QAbstractSocket::readyRead, [&]() { name = client->readAll();});
    client->waitForReadyRead();     //延时3e4 msecs
    qDebug() << "success connect";
    parseName(name, login, pass);
    client->setName(login); client->setPass(pass);

    disconnect(client, &QAbstractSocket::readyRead, 0, 0);
    qDebug() << client->getId();
    myThread* m_thread = new myThread(client);
    client->moveToThread(m_thread);
    myClient.append(client);

    for (auto i = myClient.begin(); i != myClient.end(); ++i)   //发送用户信息
    {
        auto j = myClient.begin();
        QString str = (*j)->getName().toUtf8();
        for (; j != myClient.end(); ++j)
        {
            str = "\a" + str;
            str += "\a";
        }
        qDebug() << str;
        (*i)->write(str.toUtf8());
    }

    connect(m_thread, SIGNAL(newUser(QString)), this, SIGNAL(newUser(QString)));
    connect(m_thread, SIGNAL(newMsg(QString)), this, SLOT(newMsg(QString)));
    connect(m_thread, SIGNAL(olduser(QString, qint32)), this, SLOT(old_User(QString, qint32)));

    m_thread->start();
}

void MyServer::sendFile(QByteArray& flle)
{

}

void MyServer::newMsg(QString msg)
{
    emit new_Msg(msg);
    msg = "\b" + msg;
    msg += "\b";
    for (auto i = myClient.begin(); i != myClient.end(); ++i)
        (*i)->write(msg.toUtf8());
}

void MyServer::old_User(QString user, qint32 id)
{
    emit oldUser(user);
//    for (auto i = myClient.begin(); i != myClient.end(); ++i)
//    {
//        qDebug() << (*i)->getId();
//        if ((*i)->getId() == id)
//        {
//            myClient.erase(i);
//        }
//    }
    for (auto i = myClient.begin(); i != myClient.end(); ++i)
    {
        qDebug() << (*i)->getId();
        (*i)->write(user.toUtf8());
        mySleep(100);
    }
}

