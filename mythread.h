#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include "myclient.h"
#include <QTime>

class myThread : public QThread
{
    Q_OBJECT
public:
    myThread(MyClient* client, qint32 id);
    ~myThread();
    QString intToQString(qint32 num);
    void setName(QString& str);
    QString getName();



protected:
    void run() override;

signals:
    void newUser(QString name);
    void newMsg(QString str);
    void olduser(QString user, qint32 id);

public slots:
    void sendMsg();
    void sendImg();
    void disConnect();


private:
    QThread* m_thread;
    MyClient* m_client;
    QString name;
};

#endif // MYTHREAD_H
