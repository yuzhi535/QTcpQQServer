#include "mythread.h"

MyThread::MyThread()
{
    m_thread = new QThread();
}

MyThread::~MyThread()
{

}

QString MyThread::intToQString(qint32 num)
{
    QString re;
    while (num)
    {
        re = (char)((num % 10) + 48) + re;
        num /= 10;
    }
    return re;
}


void MyThread::run()
{
    socket->setReadBufferSize(0);   //缓冲区无限大
    if (socket->isValid())
    {
        if (socket->waitForConnected())
        {
            QTime time = QTime::currentTime();
            //将新连接显示到屏幕上

            emit newUser(time.toString() + "  " + getName() + "连接上服务器!");

            connect(socket, SIGNAL(readyRead()), this, SLOT(sendMsg()));
            connect(socket, &QAbstractSocket::disconnected, this, &MyThread::disConnect);
        }
        else
        {
            qDebug() << "连接无效";
        }
    }
    exec();
}

void MyThread::sendMsg()
{
    QByteArray data = socket->readAll();
    if (!data.isEmpty())
    {
        QString msg = data;
        if (msg.size() != 0 && msg[0] != '\r')
        {
            QTime time = QTime::currentTime();
            emit newMsg(time.toString() + " " + getName() + ": " + msg);
            createFile(data, ".txt");
//            qDebug() << data.size() << "msg!";
        }
        else if (msg.size() && msg[0] == '\r')
        {
            int index = 1;
            for(; index < msg.size() && msg[index] != '\r'; ++index);
            int size = data.mid(1, index - 1).toInt();
            qDebug() << "size=" << size << "img!";
            QTime time = QTime::currentTime();
            if (size > data.mid(index + 1).size())
            {
                emit newMsg(time.toString() + " " + getName() + ": " + "发送图片失败");
            }
            else
            {
                emit new_img(data.mid(index + 1));
                msleep(100);
                emit newMsg(time.toString() + " " + getName() + ": 上传了一张图片");
            }
        }
    }
}

void MyThread::disConnect()
{
    QString msg(": 断开连接");

    emit olduser( QTime::currentTime().toString() + ": " + getName() + msg, getIndex());

    disconnect(this, SIGNAL(newMsg(QString)), 0, 0);
    disconnect(this, SIGNAL(olduser(QString, qint32)), 0, 0);
    disconnect(this, SIGNAL(new_img(QByteArray)), 0, 0);
    quit();
    wait();
}

void MyThread::createFile(QByteArray &data, QString suffix)
{
    QString th = getName();
    th = QString("/users/") + th;
    QString path = QDir::currentPath() + th;   //存储到本地文件夹
    path = QDir::toNativeSeparators(path);
    QDir dir(path);
    if (!dir.exists())
        dir.mkpath(path);
    QString curr = QDate::currentDate().toString();
    curr = "/" + curr;
    QString name(path + curr + suffix);
    QFile file;                                  //创建新文件
    name = QDir::toNativeSeparators(name);
    file.setFileName(name);
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTime time(QTime::currentTime());
    if (!file.exists())
    {
        file.close();
        file.open(QIODevice::ReadWrite | QIODevice::Append);
    }
    if (file.isOpen())
    {
        file.write(time.toString().toUtf8() + ": " + data + "\n");
        file.close();
    }
}
