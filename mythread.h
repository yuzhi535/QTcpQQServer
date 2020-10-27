#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include "myclient.h"
#include <QTime>
#include <QDir>
#include <QApplication>
#include <QDate>

class MyThread : public QThread
{
    Q_OBJECT
public:
    MyThread(MyClient* tcp, qint32 id) : socket(tcp), m_id(id) {
        m_thread = new QThread();
    }
    MyThread();
    ~MyThread();
    QString intToQString(qint32 num);
    void setName(QString& str) {name = str; }
    QString getName() { return name; }
    void setSocketDescriptor(qintptr target) { socket->setSocketDescriptor(target); }
    void createFile(QByteArray& dat, QString suffix);
    qint32 getId() {return m_id; }
    qint32 getIndex() {
        return m_id;
    }
    void setIndex(qint32 num) { m_id = num; }


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
    MyClient* socket;
    QThread* m_thread;
    QString name;
    qint32 m_id;
};

#endif // MYTHREAD_H
