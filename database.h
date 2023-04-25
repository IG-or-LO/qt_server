#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

/* Директивы имен таблицы, полей таблицы и базы данных */
#define DATABASE_HOSTNAME   "ServerDataBase"
#define DATABASE_NAME       "server.db"
//users table defines
#define USERS_TABLE              "Users"
#define USER_ID                 "User_id"
#define USER_NICKNAME           "NICKNAME"
#define USER_PASSWORD           "PASSWORD"
#define USER_NAME               "NAME"
#define USER_SURNAME            "SURNAME"
#define USER_IMAGE              "IMAGE"
#define USER_DESCRIPTION        "DESCRIPTION"
#define USER_MAIL               "MAIL"
#define USER_PHONE              "PHONE"
//ChatGroups table defines
#define CHAT_TABLE              "Chats"
#define CHAT_ID                 "CHAT_id"
#define CHAT_NAME               "CHATNAME"
#define CHAT_USER_CREATE_ID     "USER_CREATE_id"
//GroupParticipants table defines
#define PARTICIPANTS_TABLE      "ChatParticipants"
//ArchiveMessage table defines
#define MESSAGE_TABLE           "Messages"
#define MESSAGE_ID              "Message_id"
#define MESSAGE_TOID            "toID"
#define MESSAGE_FROMID          "fromID"
#define MESSAGE_CONTENT         "Content"
#define MESSAGE_DATA            "Data_create"
//StatusMessage table defines
#define STATUS_MESS_TABLE       "StatusMessages"
#define STATUS_MESSAGE          "Status_mess"

//test
#define IMAGESET                "IMAGESET"
#define NICKNAMESET             "NICKNAMESET"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();
    /* Методы для непосредственной работы с классом
     * Подключение к базе данных и вставка записей в таблицу
     * */
    bool connectToDataBase();
    bool insertIntoUsersTable(const QString &nickname, const QString &password, const QString &mail, const QByteArray &shablonImage, const QString &phone="");
    bool updatePersonalInfoUsersTable(const QString &name_user, const QString &name_add, const QString &surname_add, const QString &about);
    bool UpdatePasswordIntoMainTable(const QString &name_user,const QString &newpass);
    bool check_name_exist(const QString &nickname);
    bool check_log_In(const QString &nickname, const QString &password);
    bool insertIntoMessageTable(const QString &user_from,const QString &user_to, const QString &message, const QString &data);
    QStringList loadNewMessages(const QString &last_date,const QString &user_to_load);
    QStringList loadArchiveMessages(const QString &user_from,const QString &user_to);
    QString getPersonalInfo(const QString &name_user_to_load,const int &name_surname_or_about);
    QByteArray getImageUser(const QString &name_user_to_load);
    bool setImageUser(const QString &name_user_to_load, const QByteArray &inByteArray);
    bool updateStatusMessageTable(const QString mess_id,const QString user, const QString status);
    QString getStatusMessage(const QString mess_id,const QString user);
private:
    // Сам объект базы данных, с которым будет производиться работа
    QSqlDatabase    db;

private:
    /* Внутренние методы для работы с базой данных
     * */
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createUsersTable();
    bool createChatGroupsTable();
    bool createNewChat(const QString &name_user_create, const QString &chatname);
    bool createGroupParticipantsTable();
    bool createNewGroupParticipants(const QString chatname, const QString &userID);
    bool createArchiveMessageTable();
    bool createStatusMessageTable();
    bool createTriggerOnMessageTable();
    

    
};

#endif // DATABASE_H
