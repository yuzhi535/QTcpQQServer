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
    while (QTime::currentTime() < time1)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void MyServer::incomingConnection(qintptr socketDescriptor)
{
    MyClient* client = new MyClient();
    client->setSocketDescriptor(socketDescriptor);

    QString name, pass, login;
    connect(client, &QAbstractSocket::readyRead, [&]() { name = client->readAll();});
    client->waitForReadyRead();     //延时 3e4 msecs
    qDebug() << "success connect";
    parseName(name, login, pass);
    client->setName(login); client->setPass(pass);

    disconnect(client, &QAbstractSocket::readyRead, 0, 0);
    qDebug() << client->getId();
    myThread* m_thread = new myThread(client);
    client->moveToThread(m_thread);
    myClient.append(client);

    connect(m_thread, SIGNAL(newUser(QString)), this, SIGNAL(newUser(QString)));
    connect(m_thread, SIGNAL(newMsg(QString)), this, SLOT(newMsg(QString)));
    connect(m_thread, SIGNAL(olduser(QString, qint32)), this, SLOT(old_User(QString, qint32)));
    connect(m_thread, SIGNAL(new_img(QByteArray)), this, SLOT(sendImg(QByteArray)));

    m_thread->start();      //开始次线程
}

QString intToQString(int num)
{
    QString re;
    while (num)
    {
        re = (char)(num % 10) + 48 + re;
        num /= 10;
    }
    return re;
}

void MyServer::sendImg(QByteArray file)
{
    auto len = file.size();
    QByteArray s;
    while (len)                             //先发送图片大小，让服务端做好准备
    {
        s = (char)(len % 10 + 48) + s;
        len /= 10;
    }
    s.insert(0, '\r');
    s.append('\r');

    for (auto i = myClient.begin(); i != myClient.end(); ++i)
    {
        (*i)->write(s);
        (*i)->waitForBytesWritten();
        QThread::msleep(100);
        (*i)->write(file);
        (*i)->waitForBytesWritten();
        QThread::msleep(100);
    }
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

