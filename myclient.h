#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>

class MyClient : public QTcpSocket
{
    Q_OBJECT
public:
    MyClient();
    ~MyClient();
    void setName(QString name);
    void setPass(QString ass);
    QString getName();

    QString getPass() { return m_pass; }
    qint32 getId();
    void setId(qint32 id) { m_id = id; }

private:
    QString m_name;
    QString m_pass;
    qint32 m_id;
};

#endif // MYCLIENT_H
