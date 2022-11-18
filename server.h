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
    void doSendMessageToUsers(QString message, const QStringList &users, QString fromUsername);
    QStringList getUsersOnline() const;
    bool isNameAndPassValid(QString name,QString pass) const;
    bool isNameUsed(QString name, QString pass) const;

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
    void add_new_user_bd(QString nickname, QString password);

};

#endif // MYSERVER_H
