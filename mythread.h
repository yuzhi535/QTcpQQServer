#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>

class myThread : public QThread
{
    Q_OBJECT
public:
    myThread();
private:
    QThread* m_thread;
};

#endif // MYTHREAD_H
