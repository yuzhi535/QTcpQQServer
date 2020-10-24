#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QObject>
#include <QTcpSocket>

class MyClient : public QTcpSocket
{
    Q_OBJECT
public:
    MyClient();
//    MyClient(MyClient&);
    ~MyClient();
    void setName(QString name);
    void setPass(QString ass);
    QString getName();
    static qint32 get_id();
    static void set_id(qint32 id);
    qint32 getId();
    void setId(qint32 id);

private:
    static qint32 m_id;
    QString m_name;
    QString m_pass;
};

#endif // MYCLIENT_H
