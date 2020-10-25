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

QString MyThread::intToString(int num)
{
    QString re;
    while (num)
    {
        re = (char)(num % 10) + 48 + re;
        num /= 10;
    }
    return re;
}

void MyThread::sendMsg()
{
    QTime time = QTime::currentTime();
    QByteArray data = socket->readAll();
    if (!data.isEmpty())
    {
        QString msg = data;
        if (msg.size() != 0 && msg[0] != '\r')
        {
            emit newMsg(time.toString() + " " + getName() + ": " + msg);
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
                    data += socket->readAll();
                    if (time.secsTo(QTime::currentTime()) > 3)
                    {
                        emit newMsg(time.toString() + " " + getName() + ": " + "接收图片失败，请重发");
                        flag = false;
                        break;
                    }
                }
                if (flag)
                {
                    emit newMsg(time.toString() + " " + getName() + ": " + tr("上传了一张图片"));
                    //创建文件
                    createFile(data, ".png");
                    emit new_img(data);
                }
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
    moveToThread(QApplication::instance()->thread());
    quit();
    wait();
    deleteLater();
}

void MyThread::createFile(QByteArray &data, QString suffix)
{
    QString th = getName();
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
