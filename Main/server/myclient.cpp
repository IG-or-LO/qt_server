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
    encrypt=new Encryption();
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
        in >> _blockSize;
        qDebug() << "_blockSize now " << _blockSize<< " bytesAvalible"<<_sok->bytesAvailable();
    }
    if (_sok->bytesAvailable() < _blockSize)
        return;
    else
        _blockSize = 0;
    quint8 command;
    in >> command;
    qDebug() << "Received command " << command;

   if ((!_isAutched && command != comAutchReq && command != comRegReq))//если не подключен и
         return;                                                       //команды не на подключение
                                                                       //или регистрацию


    switch(command)
    {

        case comRegReq:
        {
            QByteArray nameBa, passBa, mailBa, phoneBa, hashKey,hashIV;
            in>>nameBa;in >> passBa;in >> mailBa;in >> phoneBa;in >> hashKey;in >> hashIV;
            QString name=encrypt->decodeMessage(hashKey,hashIV, nameBa);

            qDebug()<<"serverCommingRegLogin"<<nameBa;
            qDebug()<<"decodedLogin"<<encrypt->decodeMessage(hashKey,hashIV, nameBa);

            if (_serv->isNameUsed(encrypt->decodeMessage(hashKey,hashIV, nameBa),
                                  encrypt->decodeMessage(hashKey,hashIV, passBa),
                                  encrypt->decodeMessage(hashKey,hashIV, mailBa),
                                  encrypt->decodeMessage(hashKey,hashIV, phoneBa)))
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

        case comAutchReq:
        {
            QByteArray loginBa, passBa, hashKey, hashIV;
            in >> loginBa;in >> passBa;in >> hashKey;in >> hashIV;
            QString login=encrypt->decodeMessage(hashKey,hashIV,loginBa);
            QString pass=encrypt->decodeMessage(hashKey,hashIV,passBa);;
            qDebug()<<"COMMINGlOGIN"<<loginBa;
            qDebug()<<"decodedLogin"<<login;
            qDebug()<<"decodedPASS"<<pass;
            if(!_serv->isNameAndPassTrue(login,pass))
            {
                doSendCommand(comErrNameOrPassFalse);
                return;
            }
             _name = login;
             _isAutched = true;
             doSendCommand(comAuthSuccess);
             //временно убрал отправку списка пользователей онлайн
                     //   doSendCommand(comUsersOnline);
             emit addUserToGui(_name);
                        //отправка пользователям о входе юзера
             //_serv->doSendToAllUserJoin(_name);

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
            QByteArray user_toBa, messageBa, hashKey, hashIV;
            in >> user_toBa;
            in >> messageBa;
            in >> hashKey;
            in >> hashIV;
                        qDebug()<<"COMMINGmESS"<<messageBa;
                        qDebug()<<"decodedMESS"<<encrypt->decodeMessage(hashKey,hashIV, messageBa);
            _serv->doSendMessageToUser(encrypt->decodeMessage(hashKey,hashIV, messageBa),
                                        encrypt->decodeMessage(hashKey,hashIV,user_toBa),
                                        _name);
        }
        break;
        //
        case comGetNewMessages:
        {
            QByteArray lastDateBa,hashKey, hashIV;
            in >> lastDateBa;in >> hashKey;in>> hashIV;
            _serv->doSendNewMessages(encrypt->decodeMessage(hashKey,hashIV,lastDateBa), _name);
        }
            break;

        case comSearchUsers:
        {
            QByteArray text_searchBa, hashKey, hashIV;
            in >> text_searchBa;in >> hashKey;in >> hashIV;
            _serv->doSearchUsers(_name,
                                 encrypt->decodeMessage(hashKey,hashIV, text_searchBa));
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
            QByteArray newpassBa,hashKey, hashIV;
            in >> newpassBa; in >> hashKey;in >> hashIV;
            _serv->doUpdatePassword(_name,
                                    encrypt->decodeMessage(hashKey,hashIV, newpassBa));
        }
            break;
        case comSaveImagelUser:
        {
            QByteArray inByteArray;
            in >> inByteArray;
            _serv->doSetImageUser(_name,inByteArray);
        }
            break;

        case comGetPersonalInfo:
        {
            QString name_user_to_load;
            in>>name_user_to_load;
            _serv->doGetPersonalInfo(_name,name_user_to_load);

        }
        break;

        case comUpdateStatusMessages:
        {
            QByteArray loginUserToUpdateBa,hashKey,hashIV;
            in>> loginUserToUpdateBa; in>> hashKey; in>> hashIV;
            _serv->doUpdateStatusMess(_name,encrypt->decodeMessage(hashKey,hashIV,loginUserToUpdateBa));
        }
        break;


}

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
    case comAuthSuccess:{} //не требуется сообщения, просто отправляем команду
        break;
    default:
        break;
    }

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sok->write(block);
}
