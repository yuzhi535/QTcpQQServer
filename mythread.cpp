#include "mythread.h"

myThread::myThread(MyClient* client, qint32 id)
{
    m_thread = new QThread();
    m_client = client;
    m_client->setId(id);
}

myThread::~myThread()
{

}

void myThread::setName(QString &str)
{
    name = str;
}

QString myThread::getName()
{
    return name;
}

QString myThread::intToQString(qint32 num)
{
    QString re;
    while (num)
    {
        re = (char)((num % 10) + 48) + re;
        num /= 10;
    }
    return re;
}


void myThread::run()
{
    m_client->setReadBufferSize(0);

    if (m_client->isValid())
    {
        if (m_client->waitForConnected())
        {
            QTime time = QTime::currentTime();
            QString _info = intToQString(m_client->getId());
            //将新连接显示到屏幕上

            emit newUser(m_client->getName() + "连接上服务器!");
            connect(m_client, SIGNAL(readyRead()), this, SLOT(sendMsg()));
            connect(m_client, &QAbstractSocket::disconnected, this, &myThread::disConnect);
        }
    }
    exec();
}

#include <iostream>

void myThread::sendMsg()
{
    QString msg = m_client->readAll();
    std::cout << msg.toStdString() << std::endl;
    emit newMsg(m_client->getName() + ": " + msg);
}

void myThread::sendImg()
{

}

void myThread::disConnect()
{
    QString msg(": 断开连接");
    emit olduser(m_client->getName() + msg, m_client->getId());
}
