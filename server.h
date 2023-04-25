#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTime>
#include <QColor>

#include "database.h"
#include "myclient.h"

class MyClient;


class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(QWidget *widget = 0, QObject *parent = 0);
    bool doStartServer(QHostAddress addr, qint16 port);
    void doSendToAllUserJoin(QString name);
    void doSendToAllUserLeft(QString name);
    void doSendToAllMessage(QString message, QString fromUsername);
    void doSendToAllServerMessage(QString message);
    void doSendServerMessageToUsers(QString message, const QStringList &users);
    void doSendMessageToUsers(QString message, const QStringList &users_to, QString fromUsername);
    QStringList getUsersOnline() const;
    bool isNameUsed(QString name, QString pass, QString mail, QString phone="") const;
    bool isNameAndPassTrue(QString name, QString pass) const;
    void doSendNewMessages(QString last_date, QString name_user_to_load);
    void doSendArchive( QString name, QString name_user_to_load) const;
    void doUpdatePersonalInfo(QString name_user, QString name_add,QString surname_add,QString aboutme_add) const;
    void doUpdatePassword(QString name_user, QString newpass) const;
    void doGetPersonalInfo(QString name_user, QString name_user_to_load) const;
   // void doGetPersonalInfoAterAuth(QString name_user) const;
    void doSetImageUser(QString name_user, QByteArray inByteArray) const;
    QByteArray doGetImageUser(QString name_user_to_load) const;


signals:
    void addLogToGui(QString string, QColor color = Qt::black);

public slots:
    void onMessageFromGui(QString message, const QStringList &users);
    void onRemoveUser(MyClient *client);

protected:
    void incomingConnection(qintptr handle);

private:
    DataBase        *db;
    QList<MyClient *> _clients;
    QWidget *_widget;

    QByteArray _shablonImageByteArray;
    void getShablonImage();

};

#endif // MYSERVER_H
