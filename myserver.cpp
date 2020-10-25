#include "myserver.h"
#include <QString>

MyServer::MyServer()
{
    myServer = new QTcpServer();
    index = 10000;
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
        if (str[i] == '\t' && flag)
        {
            flag = false;
            name = str.mid(j, i - j);
            j = i + 1;
        }
        else if (str[i] == '\t')
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
    MyClient** tmp_client = new MyClient*();
    *tmp_client = new MyClient();
    MyClient* client = *tmp_client;    //注意变量生命周期
    client->setSocketDescriptor(socketDescriptor);
    qint32 id = getIndex();
    MyThread* m_thread = new MyThread(client, id);
    (*tmp_client)->setId(id);


    QString name, pass, login;
    client->waitForReadyRead();     //延时 3e4 msecs，进行阻塞
    name = client->readAll();
    qDebug() << "success connect";
    parseName(name, login, pass);
    client->setName(login); client->setPass(pass);


    m_thread->setName(login);
    user[login] = pass;

    login = QTime::currentTime().toString() + ":  " + login + ":  连接服务器";

    client->write(login.toUtf8());

    emit newUser(login);

    qDebug() << (*tmp_client)->getId() << "asdfasdfasdfasdfasdf";

    myClient.push_back(*tmp_client);

//    (*tmp_client)->moveToThread(m_thread);

    connect(m_thread, SIGNAL(newMsg(QString)), this, SLOT(newMsg(QString)));
    connect(m_thread, SIGNAL(olduser(QString, qint32)), this, SLOT(old_User(QString, qint32)));
    connect(m_thread, SIGNAL(new_img(QByteArray)), this, SLOT(sendImg(QByteArray)));

    for (auto i = myClient.begin(); i != myClient.end(); ++i)
        qDebug() << (*i)->getId();

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
    emit showImg(file);



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
    for (auto i = myClient.begin(); i != myClient.end(); ++i)
    {
        if ((*i)->getId() == id)
        {
            myClient.erase(i);
        }
        if (myClient.size() == 0)
            break;
    }
    for (auto i = myClient.begin(); i != myClient.end(); ++i)
    {
        qDebug() << (*i)->getId();
        (*i)->write(user.toUtf8());
        mySleep(100);
    }
}

