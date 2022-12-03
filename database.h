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

#define SERVER              "ServerTable"
#define SERVER_NICKNAME     "NICKNAME"
#define SERVER_PASSWORD     "PASSWORD"
#define SERVER_NAME         "NAME"
#define SERVER_SURNAME      "SURNAME"
#define SERVER_ABOUT        "ABOUT"

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
    bool inserIntoMainTable(const QString nickname, const QString password);
    bool UpdatePersonalInfoIntoMainTable(QString name_user,QString name_add,QString surname_add,QString about);
    bool UpdatePasswordIntoMainTable(QString name_user,QString newpass);
    bool check_name_exist(const QString nickname);
    bool check_log_In(const QString nickname, const QString password);
    bool createArchiveMessageTable(QString user_name);
    bool InsertIntoArchiveMessageTable(QString user_name_table,QString user_id_name,QString message);
    QString loadArchiveMessages(QString user_name_table,QString user_id_name);
    QString getPersonalInfo(QString name_user_to_load, int name_surname_or_about);

private:
    // Сам объект базы данных, с которым будет производиться работа
    QSqlDatabase    db;

private:
    /* Внутренние методы для работы с базой данных
     * */
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createMainTable();

    

    
};

#endif // DATABASE_H
