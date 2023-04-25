#include "server.h"
#include <QDataStream>
#include <QDateTime>

#include <QBuffer>
#include <QPixmap>

Server::Server(QWidget *widget, QObject *parent) :QTcpServer(parent)
{
    _widget = widget;
    db=new DataBase;
    db->connectToDataBase();
    getShablonImage();
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
    out << (quint16)0 << MyClient::comUserJoin << name <<doGetImageUser(name);
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
        if (_clients.at(i)->getAutched() && _clients.at(i)->getName()!=fromUsername)
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

void Server::doSendMessageToUsers(QString message, const QStringList &users_to, QString fromUsername)
{
    QDateTime date;
    date=date.currentDateTime();

    qDebug()<<"next step try to insert this mess to db";
    db->insertIntoMessageTable(fromUsername,users_to[0],message,date.toString());
     //send to user_to
    QByteArray block, blockToSender;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << MyClient::comMessageToUsers << fromUsername << message<<date;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));



    QDataStream outToSender(&blockToSender, QIODevice::WriteOnly);
    outToSender << (quint16)0 << MyClient::comMessageToUsers << users_to.join(",") << message<<date;
    outToSender.device()->seek(0);
    outToSender << (quint16)(blockToSender.size() - sizeof(quint16));

    for (int j = 0; j < _clients.length(); ++j)
        if (users_to.contains(_clients.at(j)->getName()))
            _clients.at(j)->_sok->write(block);
}

QStringList Server::getUsersOnline() const
{
    QStringList l;
    foreach (MyClient * c, _clients)
        if (c->getAutched())
            l << c->getName();
    return l;
}



bool Server::isNameUsed(QString name, QString pass, QString mail, QString phone) const
{
    qDebug()<<"query check this name="<<name;
    if( db->check_name_exist(name)){
        qDebug()<<"query return true,name is used already";
        return true;
    }
    else
    {
        db->insertIntoUsersTable(name,pass,mail,_shablonImageByteArray, phone);
        return false;
    }

}

bool Server::isNameAndPassTrue(QString name, QString pass) const
{
    qDebug()<<"query check this name="<<name;
    if( db->check_log_In(name,pass)){
        qDebug()<<"query return true, exist log and pass";
        return true;
    }
    else
    {
        qDebug()<<"query return false, no such log and pass";
        return false;
    }
}

void Server::doSendNewMessages(QString last_date, QString name_user_to_load)
{
    QStringList malesDataStatusFrom;
    qDebug()<<"try to load new messages to "<<name_user_to_load;
    malesDataStatusFrom=db->loadNewMessages(last_date,name_user_to_load);
    qDebug()<<"new messages: "<<malesDataStatusFrom;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << MyClient::comGetNewMessages << malesDataStatusFrom;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    for (int i = 0; i < _clients.length(); ++i)
        if (_clients.at(i)->getName()==name_user_to_load)
        {
            _clients.at(i)->_sok->write(block);
            return;
        }
}

void Server::doSendArchive(QString name_user, QString name_user_to_load) const
{
    QStringList malesDataStatusFrom;
    qDebug()<<"try to load archive from db...";
    malesDataStatusFrom=db->loadArchiveMessages(name_user,name_user_to_load);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << MyClient::comLoadArchive << malesDataStatusFrom;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    for (int i = 0; i < _clients.length(); ++i)
        if (_clients.at(i)->getName()==name_user)
        {
            _clients.at(i)->_sok->write(block);
            return;
        }
}

void Server::doUpdatePersonalInfo(QString name_user, QString name_add, QString surname_add, QString aboutme_add) const
{
    db->updatePersonalInfoUsersTable(name_user,name_add,surname_add,aboutme_add);
}

void Server::doUpdatePassword(QString name_user, QString newpass) const
{
    db->UpdatePasswordIntoMainTable(name_user,newpass);
}

void Server::doGetPersonalInfo(QString name_user, QString name_user_to_load) const
{
    QString name;
    QString surname;
    QString about;
    QByteArray inByteArray;
    name=db->getPersonalInfo(name_user_to_load,0);
    surname=db->getPersonalInfo(name_user_to_load,1);
    about=db->getPersonalInfo(name_user_to_load,2);
    inByteArray=db->getImageUser(name_user_to_load);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    //
    out << (quint16)0 << MyClient::comGetPersonalInfo << name<<surname<<about<<inByteArray;
    //
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    //
    for (int i = 0; i < _clients.length(); ++i)
        if (_clients.at(i)->getName()==name_user)
        {
            _clients.at(i)->_sok->write(block);
            return;
        }
}

//void Server::doGetPersonalInfoAterAuth(QString name_user) const
//{
//    QString name;
//    QString surname;
//    QString about;
//    QByteArray inByteArray;
//    name=db->getPersonalInfo(name_user,0);
//    surname=db->getPersonalInfo(name_user,1);
//    about=db->getPersonalInfo(name_user,2);
//    inByteArray=db->getImageUser(name_user);
//    QByteArray block;
//    QDataStream out(&block, QIODevice::WriteOnly);
//    //
//    out << (quint16)0 << MyClient::comAuthSuccess << name<<surname<<about<<inByteArray;
//    //
//    out.device()->seek(0);
//    out << (quint16)(block.size() - sizeof(quint16));
//    //
//    for (int i = 0; i < _clients.length(); ++i)
//        if (_clients.at(i)->getName()==name_user)
//        {
//            _clients.at(i)->_sok->write(block);
//            return;
//        }
//}

void Server::doSetImageUser(QString name_user, QByteArray inByteArray) const
{
    db->setImageUser(name_user,inByteArray);
}

QByteArray Server::doGetImageUser(QString name_user_to_load) const
{
   return db->getImageUser(name_user_to_load);
}

void Server::incomingConnection(qintptr handle)
{
    //
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

void Server::getShablonImage()
{
    QPixmap image=QPixmap(":/resources/images/shablon_image.png");
    QBuffer inBuffer( &_shablonImageByteArray );
    inBuffer.open(QIODevice::WriteOnly);
    image.save(&inBuffer, "PNG");
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
