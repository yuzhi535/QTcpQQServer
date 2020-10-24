#include "myclient.h"

qint32 MyClient::m_id = 10000;

MyClient::MyClient()
{
    ++m_id;
}

MyClient::~MyClient()
{

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

void MyClient::setId(qint32 id)
{
    set_id(id);
}

qint32 MyClient::getId()
{
    return get_id();
}

void MyClient::set_id(qint32 id)
{
    m_id = id;
}

qint32 MyClient::get_id()
{
    return m_id;
}
