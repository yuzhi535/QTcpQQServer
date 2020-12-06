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
    socket->setReadBufferSize(0);                //缓冲区无限大
    if (socket->isValid())
    {
        if (socket->waitForConnected())
        {
            QTime time = QTime::currentTime();
            //将新连接显示到屏幕上

            emit newUser(QDate::currentDate().toString() + ". " + time.toString() + "$ " + getName() + "连接上服务器! ");

            connect(socket, SIGNAL(readyRead()), this, SLOT(sendMsg()));
            connect(socket, &QAbstractSocket::disconnected, this, &MyThread::disConnect);
        }
        else
        {
            qDebug() << "连接无效";
            disConnect();
        }
    }
    exec();
}

void MyThread::sendMsg()
{
    static int m_flag = 0;
    QByteArray data = socket->readAll();
    if (!data.isEmpty())
    {
        QString msg = data;
        if (msg.size() != 0 && m_flag == 0 && msg[0] != '\r')   //如果是接收到文本
        {
            QTime time = QTime::currentTime();
            emit newMsg(QDate::currentDate().toString() + ". " + time.toString() + "$ " + getName() + ": " + msg);
            createFile(data, ".txt");
        }
        else if (msg.size() && (msg[0] == '\r' || m_flag))     //如果接收到的是图片
        {
            static QByteArray recvData;
            static int size;
            if (!m_flag)     //若是接收的图片的第一部分
            {
                int index = 1;
                for(; index < msg.size() && msg[index] != '\r'; ++index);

                recvData += data.mid(index + 1);
                size = data.mid(1, index - 1).toInt();
                qDebug() << "size=" << size << "img!";
                if (size > recvData.size())
                {
                    ++m_flag;
                    qDebug() << "real=" << recvData.size();
                }
                else
                {
                    m_flag = 0;
                    size = 0;
                    emit new_img(recvData);
                    msleep(100);
                    emit newMsg(QDateTime::currentDateTime().toString() + " " + getName() + ": 上传了一张图片");
                    auto file = recvData;
                    createFile(file, ".png");
                    recvData.clear();
                }
            }
            else
            {
                recvData += data;
                if (size > recvData.size())
                {
                    if (m_flag > 5)
                    {
                        m_flag = 0;
                        size = 0;
                        recvData.clear();
                        emit newMsg(QTime::currentTime().toString() + " " + getName() + ": " + "发送图片失败");
                        qDebug() << "=real" << recvData.size();
                    }
                    else
                    {
                        qDebug() << "m_flag " << m_flag;
                        ++m_flag;
                        qDebug() << "real=" << recvData.size();
                    }
                }
                else
                {
                    m_flag = 0;
                    size = 0;
                    emit new_img(recvData);
                    msleep(100);
                    emit newMsg(QDateTime::currentDateTime().toString() + " " + getName() + ": 上传了一张图片");
                    auto file = recvData;
                    createFile(file, ".png");
                    recvData.clear();
                }
            }
        }
    }
}

void MyThread::disConnect()
{
    QString msg("断开连接");

    emit olduser( QDate::currentDate().toString() + ". " +
                  QTime::currentTime().toString() + "$ " + getName() + ": " + msg, getIndex());

    //全部断连
    disconnect(this, SIGNAL(newMsg(QString)), 0, 0);
    disconnect(this, SIGNAL(olduser(QString, qint32)), 0, 0);
    disconnect(this, SIGNAL(new_img(QByteArray)), 0, 0);
    quit();
    wait();
}

void MyThread::createFile(QByteArray& data, QString suffix)
{
    QString th = getName();
    th = QString("/users/") + th;
    QString path = QDir::currentPath() + th;     //存储到本地文件夹
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
    if (file.isOpen() && suffix == ".txt")
    {
        file.write( QDate::currentDate().toString().toUtf8() + ". " + time.toString().toUtf8() + "$ " + data + "\n");
        file.close();
    }
    else if (file.isOpen() && suffix == ".png")
    {
        file.write(data);
        file.close();
    }
}
