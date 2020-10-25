#include "myclient.h"

MyClient::MyClient()
{
    m_id = 0;
}

MyClient::~MyClient()
{
    delete socket;
}

void MyClient::setName(QString name)
{
    m_name = name;
}

QString MyClient::getName()
{
    return m_name;
}

void MyClient::setPass(QString ass)
{
    m_pass = ass;
}

qint32 MyClient::getId()
{
    return m_id;
}
