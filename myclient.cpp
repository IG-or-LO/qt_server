#include "myclient.h"
#include <QWidget>
#include <QMessageBox>

const QString MyClient::constNameUnknown = QString(".Unknown");

MyClient::MyClient(int desc, Server *serv, QObject *parent) :QObject(parent)
{
    _serv = serv;
    _isAutched = false;
    _name = constNameUnknown;
    _blockSize = 0;
    _sok = new QTcpSocket(this);
    _sok->setSocketDescriptor(desc);
    connect(_sok, SIGNAL(connected()), this, SLOT(onConnect()));
    connect(_sok, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
    connect(_sok, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(_sok, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));

    qDebug() << "Client connected" << desc;
}

MyClient::~MyClient()
{

}

void MyClient::onConnect()
{
    //never calls, socket already connected to the tcpserver
    //we just binding to this socket here: _sok->setSocketDescriptor(desc);
}

void MyClient::onDisconnect()
{
    qDebug() << "Client disconnected";
    //
    if (_isAutched)
    {
        emit removeUserFromGui(_name);
        _serv->doSendToAllUserLeft(_name);
        emit removeUser(this);
    }
    deleteLater();
}

void MyClient::onError(QAbstractSocket::SocketError socketError) const
{
    QWidget w;
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(&w, "Error", "The host was not found");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(&w, "Error", "The connection was refused by the peer.");
        break;
    default:
        QMessageBox::information(&w, "Error", "The following error occurred: "+_sok->errorString());
    }
}

void MyClient::onReadyRead()
{
    QDataStream in(_sok);
    if (_blockSize == 0) {
        if (_sok->bytesAvailable() < (int)sizeof(quint16))
     //    if (_sok->bytesAvailable() < (int)sizeof(unsigned int))
            return;
        //
        in >> _blockSize;
        qDebug() << "_blockSize now " << _blockSize<< " bytesAvalible"<<_sok->bytesAvailable();
    }
    //
    if (_sok->bytesAvailable() < _blockSize)
        return;
    else
        //
        _blockSize = 0;
    //3
    quint8 command;
    in >> command;
    qDebug() << "Received command " << command;
    //
   if (!_isAutched && command != comAutchReq && command != comRegReq)
        return;

    switch(command)
    {

        case comRegReq:
        {
            QString name;
            QString pass;
            QString mail;
            QString phone;
            in>>name;
            in>>pass;
            in>>mail;
            in>>phone;
            //
            if (_serv->isNameUsed(name,pass,mail,phone))
            {
                //
                doSendCommand(comErrNameUsed);
                return;
            }
            doSendCommand(comRegSuccess);
            _name = name;
            //
            emit addUserToGui(name);
        }
        break;
        //
        case comAutchReq:
        {
            //
            QString name;
            QString pass;//new line pass
            in >> name;
            in >> pass;

            //
            if (!_serv->isNameAndPassTrue(name,pass))
            {
                //
                doSendCommand(comErrNameOrPassFalse);
                return;
            }
            //
            _name = name;
            _isAutched = true;
            //
          //  _serv->doGetPersonalInfoAterAuth(_name);
            doSendCommand(comAuthSuccess);

            //временно убрал отправку списка пользователей онлайн
         //   doSendCommand(comUsersOnline);
            //
            emit addUserToGui(name);
            //
            _serv->doSendToAllUserJoin(_name);
        }
        break;
        //
        case comMessageToAll:
        {
            QString message;
            in >> message;
            //
            _serv->doSendToAllMessage(message, _name);
            //
            emit messageToGui(message, _name, QStringList());
        }
        break;
        //
        case comMessageToUsers:
        {
            QString user_to;
            in >> user_to;
            QString message;
            in >> message;
            //
            QStringList user_toList = user_to.split(",");
            //
            _serv->doSendMessageToUsers(message, user_toList, _name);
            //
            emit messageToGui(message, _name, user_toList);
        }
        break;
        //
    case comGetNewMessages:
    {
        QString lastDate;
        in>>lastDate;
        _serv->doSendNewMessages(lastDate,_name);
    }
        break;

    case comLoadArchive:
    {
        QString name_user;
        QString name_user_to_load;
        in>>name_user;
        in>>name_user_to_load; 
        _serv->doSendArchive(name_user, name_user_to_load);
        //
    }
    break;

    case comSavePersonalInfo:
    {
        QString name_user;
        QString name_add;
        QString surname_add;
        QString aboutme_add;
        in>>name_user;
        in>>name_add;
        in>>surname_add;
        in>>aboutme_add;
        _serv->doUpdatePersonalInfo(name_user,name_add,surname_add,aboutme_add);
    }
        break;
    case comUpdatePassword:
    {
        QString name_user;
        QString newpass;

        in>>name_user;
        in>>newpass;
        _serv->doUpdatePassword(name_user,newpass);
    }
        break;
    case comSaveImagelUser:
    {
        QString name_user;
        QByteArray inByteArray;
        in>>name_user;
        in>>inByteArray;
        _serv->doSetImageUser(name_user,inByteArray);
    }
        break;

    case comGetPersonalInfo:
    {
        QString name_user;
        QString name_user_to_load;
        in>>name_user;
        in>>name_user_to_load;
        _serv->doGetPersonalInfo(name_user,name_user_to_load);

    }
        break;

}
    //for (long long i = 0; i < 4000000000; ++i){}
}

void MyClient::doSendCommand(quint8 comm) const
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8) comm;

    switch (comm) {
    case comUsersOnline:
    {

        QStringList l = _serv->getUsersOnline();
        QByteArray byteImageArray;
        QString s;
        for (int i = 0,k=0; i < l.length(); ++i,++k)
            if (l.at(i) != _name)
            {
                s += l.at(i)+(QString)",";
            }
        s.remove(s.length()-1, 1);
        out << s;
        for (int i = 0,k=0; i < l.length(); ++i,++k)
            if (l.at(i) != _name)
            {
                byteImageArray=_serv->doGetImageUser(l.at(i));
                out << byteImageArray;
            }
    }
        break;
    case comAuthSuccess:
    {

    }
        break;
    default:
        break;
    }

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sok->write(block);
}
