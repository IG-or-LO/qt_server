#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QThreadPool>
#include <QRegExp>

#include "server.h"
#include <encryption.h>

class Server;

class MyClient : public QObject
{
    friend class MyServer;
    Q_OBJECT

public:
    QTcpSocket *_sok;

    static const QString constNameUnknown;
    static const quint8 comRegReq=0;
    static const quint8 comAutchReq = 1;
    static const quint8 comUsersOnline = 2;
    static const quint8 comUserJoin = 3;
    static const quint8 comUserLeft = 4;
    static const quint8 comMessageToAll = 5;
    static const quint8 comMessageToUsers = 6;
    static const quint8 comPublicServerMessage = 7;
    static const quint8 comPrivateServerMessage = 8;
    static const quint8 comAuthSuccess = 9;
    static const quint8 comLoadArchive=10;
    static const quint8 comSavePersonalInfo = 11;
    static const quint8 comUpdatePassword = 12;
    static const quint8 comGetPersonalInfo = 13;
    static const quint8 comSaveImagelUser = 14;
    static const quint8 comRegSuccess=15;
    static const quint8 comGetNewMessages=16;
    static const quint8 comUpdateStatusMessages=17;
    static const quint8 comSearchUsers=19;
    static const quint8 comErrNameUsed = 201;
    static const quint8 comErrNameOrPassFalse = 202;

    explicit MyClient(int desc, Server *serv, QObject *parent = 0);
    ~MyClient();

    void setName(QString name) {_name = name;}
    QString getName() const {return _name;}
    bool getAutched() const {return _isAutched;}
    void doSendCommand(quint8 comm) const;

signals:
    void addUserToGui(QString name);
    void removeUserFromGui(QString name);
    void removeUser(MyClient *client);
    void messageToGui(QString message, QString from, const QStringList &users);

private slots:
    void onConnect();
    void onDisconnect();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError socketError) const;

private:
    Server *_serv;
    Encryption *encrypt;
    quint16 _blockSize;
    QString _name;
    bool _isAutched;
};

#endif // MYCLIENT_H

