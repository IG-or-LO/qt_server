#include "database.h"
#include <QSqlRecord>

DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

DataBase::~DataBase()
{

}

/* Методы для подключения к базе данных
 * */
bool DataBase::connectToDataBase()
{
    /* Перед подключением к базе данных производим проверку на её существование.
     * В зависимости от результата производим открытие базы данных или её восстановление
     * */
    if(!QFile("./" DATABASE_NAME).exists()){
        this->restoreDataBase();
        return false;
    } else {
        this->openDataBase();
        return true;
    }
}

/* Методы восстановления базы данных
 * */
bool DataBase::restoreDataBase()
{
    if(this->openDataBase()){
        if(!this->createMainTable()){
            return false;
        } else {
            return true;
        }
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    return false;
}

/* Метод для открытия базы данных
 * */
bool DataBase::openDataBase()
{
    /* База данных открывается по заданному пути
     * и имени базы данных, если она существует
     * */
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName("./" DATABASE_NAME);
    if(db.open()){
        return true;
    } else {
        return false;
    }
}

/* Методы закрытия базы данных
 * */
void DataBase::closeDataBase()
{
    db.close();
}

/* Метод для создания таблицы пользователей в базе данных
 * */
bool DataBase::createMainTable()
{
    /* В данном случае используется формирование сырого SQL-запроса
     * с последующим его выполнением.
     * */
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " SERVER  " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            SERVER_NICKNAME     " VARCHAR   NOT NULL,"
                            SERVER_PASSWORD     " VARCHAR   NOT NULL"
                             " )"
                    )){
        qDebug() << "DataBase: error of create " <<SERVER;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}



/* Метод для вставки записи в таблицу пользователей
 * */
bool DataBase::inserIntoMainTable(const QString nickname, const QString password)
{

    QSqlQuery query;
    /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
    query.prepare("INSERT INTO " SERVER " ( " SERVER_NICKNAME ", "
                                              SERVER_PASSWORD ") "
                  "VALUES (:NICKNAME, :PASSWORD )");
    query.bindValue(":NICKNAME",  nickname);
    query.bindValue(":PASSWORD",  password);
    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert into " << SERVER;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::check_user(const QString nickname, const QString password)
{
    QSqlQuery query;
    QString names=nullptr;
    QString str = QString("SELECT * FROM ServerTable WHERE EXISTS "
                          "(SELECT NICKNAME FROM ServerTable WHERE NICKNAME= '"+nickname+"')");

    query.prepare(str);
    query.exec();
   qDebug()<< query.lastError();

    if(query.isActive()){
        qDebug()<<"query is active now";
        while(query.next())
        {
            names=query.value(0).toString();
        }
    }
    else
        qDebug()<<"ERROR:"<<query.lastError();

    return names==nullptr?false:true;
//    if(names==nullptr)
//        return false;
//    else
//        return true;
//    if(query.value(0) !=0)
//        return true;
//    else
//        return false;

//    while(query.next()){
//        return true;
//    }
//    return false;
    
}
