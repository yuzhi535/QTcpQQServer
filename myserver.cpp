#include "myserver.h"

MyServer::MyServer()
{
    myServer = new QTcpServer();
}

MyServer::~MyServer()
{

}

void MyServer::incomingConnection(qintptr socketDescriptor)
{

}



void MyServer::addInfo(QString data, QString id)
{

}

void MyServer::closeClient()
{

}

void MyServer::sendFile(QByteArray& flle, QString id)
{

}

