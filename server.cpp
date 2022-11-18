#include "server.h"
#include <QDataStream>

Server::Server(QWidget *widget, QObject *parent) :QTcpServer(parent)
{
    _widget = widget;
    db=new DataBase;
    db->connectToDataBase();
}

void Server::add_new_user_bd(QString nickname, QString password)
{
    db->inserIntoMainTable(nickname,password);
}


bool Server::doStartServer(QHostAddress addr, qint16 port)
{
    if (!listen(addr, port))
    {
        qDebug() << "Server not started at" << addr << ":" << port;
        return false;
    }
    qDebug() << "Server started at" << addr << ":" << port;
    return true;
}

void Server::doSendToAllUserJoin(QString name)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    //
    out << (quint16)0 << MyClient::comUserJoin << name;
    //
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    //
    for (int i = 0; i < _clients.length(); ++i)
        if (_clients.at(i)->getName() != name && _clients.at(i)->getAutched())
            _clients.at(i)->_sok->write(block);
}

void Server::doSendToAllUserLeft(QString name)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << MyClient::comUserLeft << name;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    for (int i = 0; i < _clients.length(); ++i)
        if (_clients.at(i)->getName() != name && _clients.at(i)->getAutched())
            _clients.at(i)->_sok->write(block);
}

void Server::doSendToAllMessage(QString message, QString fromUsername)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << MyClient::comMessageToAll << fromUsername << message;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    for (int i = 0; i < _clients.length(); ++i)
        if (_clients.at(i)->getAutched())
            _clients.at(i)->_sok->write(block);
}

void Server::doSendToAllServerMessage(QString message)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << MyClient::comPublicServerMessage << message;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    for (int i = 0; i < _clients.length(); ++i)
        if (_clients.at(i)->getAutched())
            _clients.at(i)->_sok->write(block);
}

void Server::doSendServerMessageToUsers(QString message, const QStringList &users)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << MyClient::comPrivateServerMessage << message;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    for (int j = 0; j < _clients.length(); ++j)
        if (users.contains(_clients.at(j)->getName()))
            _clients.at(j)->_sok->write(block);
}

void Server::doSendMessageToUsers(QString message, const QStringList &users, QString fromUsername)
{
    QByteArray block, blockToSender;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << MyClient::comMessageToUsers << fromUsername << message;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    //����, ��� �������� ����� ��������� �� ��� ���, � ������ ���, ���� �� ��������
    QDataStream outToSender(&blockToSender, QIODevice::WriteOnly);
    outToSender << (quint16)0 << MyClient::comMessageToUsers << users.join(",") << message;
    outToSender.device()->seek(0);
    outToSender << (quint16)(blockToSender.size() - sizeof(quint16));
    for (int j = 0; j < _clients.length(); ++j)
        if (users.contains(_clients.at(j)->getName()))
            _clients.at(j)->_sok->write(block);
        else if (_clients.at(j)->getName() == fromUsername)
            _clients.at(j)->_sok->write(blockToSender);
}

QStringList Server::getUsersOnline() const
{
    QStringList l;
    foreach (MyClient * c, _clients)
        if (c->getAutched())
            l << c->getName();
    return l;
}

bool Server::isNameAndPassValid(QString name,QString pass) const
{
    if (name.length() > 20 || name.length() < 5)
        return false;
    if (pass.length() > 20 || pass.length() < 5)
        return false;

    QRegExp r("[A-Za-z0-9_]+");
    if(r.exactMatch(name) && r.exactMatch(pass))
        return true;
    else
        return false;

}

bool Server::isNameUsed(QString name, QString pass) const
{
    qDebug()<<"query check this name="<<name;
    if( db->check_user(name,pass)){
        qDebug()<<"query return true";
        return true;
    }
    else
    {qDebug()<<"query return false";
        db->inserIntoMainTable(name,pass);
        return false;
    }

    //before bd
//    for (int i = 0; i < _clients.length(); ++i)
//        if (_clients.at(i)->getName() == name)
//            return true;
//    return false;
}

void Server::incomingConnection(qintptr handle)
{
    //������� �������
    MyClient *client = new MyClient(handle, this, this);
//    if (_widget != 0)
//    {
//        connect(client, SIGNAL(addUserToGui(QString)), _widget, SLOT(onAddUserToGui(QString)));
//        connect(client, SIGNAL(removeUserFromGui(QString)), _widget, SLOT(onRemoveUserFromGui(QString)));
//        connect(client, SIGNAL(messageToGui(QString,QString,QStringList)), _widget, SLOT(onMessageToGui(QString,QString,QStringList)));
//    }
    connect(client, SIGNAL(removeUser(MyClient*)), this, SLOT(onRemoveUser(MyClient*)));
    _clients.append(client);
}



void Server::onRemoveUser(MyClient *client)
{
    _clients.removeAt(_clients.indexOf(client));
}

void Server::onMessageFromGui(QString message, const QStringList &users)
{
    if (users.isEmpty())
        doSendToAllServerMessage(message);
    else
        doSendServerMessageToUsers(message, users);
}
