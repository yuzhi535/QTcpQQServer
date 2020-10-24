#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include "myclient.h"
#include <QTime>
#include <QDir>

class myThread : public QThread
{
    Q_OBJECT
public:
    myThread(MyClient* client, qint32 id);
    myThread(MyClient* client);
    myThread();
    ~myThread();
    QString intToQString(qint32 num);
    void setName(QString& str);
    QString getName();
    void setSocketDescriptor(qintptr target);
    void createFile(QByteArray& dat, QString suffix);
    QString intToString(int num);


protected:
    void run() override;

signals:
    void newUser(QString name);
    void newMsg(QString str);
    void olduser(QString user, qint32 id);
    void new_img(QByteArray img);

public slots:
    void sendMsg();
    void disConnect();


private:
    QThread* m_thread;
    MyClient* m_client;
    QString name;
};

#endif // MYTHREAD_H
