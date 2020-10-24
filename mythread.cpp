#include "mythread.h"

myThread::myThread(MyClient* client, qint32 id)
{
    m_thread = new QThread();
    m_client = client;
    m_client->setId(id);
}

myThread::myThread(MyClient* client)
{
    m_thread = new QThread();
    m_client = client;
}

myThread::myThread()
{
    m_thread = new QThread();
    m_client = new MyClient();
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
//    m_client->setReadBufferSize(0);

    if (m_client->isValid())
    {
        if (m_client->waitForConnected())
        {
            QTime time = QTime::currentTime();
            //将新连接显示到屏幕上

            emit newUser(time.toString() + "  " + m_client->getName() + "连接上服务器!");

            connect(m_client, SIGNAL(readyRead()), this, SLOT(sendMsg()));
            connect(m_client, &QAbstractSocket::disconnected, this, &myThread::disConnect);
        }
    }
    exec();
}

QString myThread::intToString(int num)
{
    QString re;
    while (num)
    {
        re = (char)(num % 10) + 48 + re;
        num /= 10;
    }
    return re;
}

void myThread::sendMsg()
{
    QTime time = QTime::currentTime();
    QByteArray data = m_client->readAll();
    if (!data.isEmpty())
    {
        QString msg = data;
        if (msg.size() != 0 && msg[0] != '\r')
        {
            emit newMsg(time.toString() + " " + m_client->getName() + ": " + msg);
            createFile(data, ".txt");
        }
        else
        {
            if (msg.at(0) == '\r')
            {
                int size = 0;
                for (int i = 1; i < data.size() && data.at(i) != '\r'; ++i)
                {
                    size = size * 10 - '0' + data.at(i);
                }
                qDebug() << size;
                data.clear();
                msleep(100);
                bool flag = true;
                QTime time(QTime::currentTime());
                while (size > data.size())
                {
                    if (time.secsTo(QTime::currentTime()) > 3)
                    {
                        emit newMsg(time.toString() + " " + m_client->getName() + ": " + "接收图片失败，请重发");
                        flag = false;
                        break;
                    }
                    data += m_client->readAll();
                }
                if (flag)
                {
                    emit newMsg(time.toString() + " " + m_client->getName() + ": " + tr("上传了一张图片"));
                    //创建文件
                    createFile(data, ".png");
                    emit new_img(data);
                }
            }
        }
    }
}

void myThread::disConnect()
{
    QString msg(": 断开连接");
    emit olduser(m_client->getName() + msg, m_client->getId());
//    this->deleteLater();
}

void myThread::setSocketDescriptor(qintptr target)
{
    m_client->setSocketDescriptor(target);
}

void myThread::createFile(QByteArray &data, QString suffix)
{
    QString th = m_client->getName();
    th = QString("/") + th;
    QString path = QDir::currentPath() + th;   //存储到本地文件夹
    path = QDir::toNativeSeparators(path);
    QDir dir(path);
    if (!dir.exists())
        dir.mkpath(path);
    QTime time(QTime::currentTime());
    QString curr = intToQString(QTime(0, 0, 0).secsTo(time));
    curr = "/" + curr;
    QString name(path + curr + suffix);
    QFile file;                                  //创建新文件
    name = QDir::toNativeSeparators(name);
    file.setFileName(name);
    file.open(QIODevice::WriteOnly);
    if (!file.exists())
    {
        file.close();
        file.open(QIODevice::ReadWrite);
    }
    if (file.isOpen())
    {
        file.write(data);
        file.close();
    }
}
