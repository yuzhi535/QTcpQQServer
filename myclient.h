#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QObject>
#include <QTcpSocket>

class MyClient : public QTcpSocket
{
    Q_OBJECT
public:
    MyClient();
    ~MyClient();
    void setId(QString name);
    QString getId();


private:
    QString id;
};

#endif // MYCLIENT_H
