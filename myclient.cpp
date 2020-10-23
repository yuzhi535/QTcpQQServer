#include "myclient.h"

MyClient::MyClient()
{

}

MyClient::~MyClient()
{

}

void MyClient::setId(QString name)
{
    id = name;
}

QString MyClient::getId()
{
    return id;
}
