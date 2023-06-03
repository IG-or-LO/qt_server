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
    if(this->openDataBase())
    {
        if(createUsersTable() && createChatGroupsTable() && createGroupParticipantsTable()
                && createArchiveMessageTable() && createStatusMessageTable()
                && createTriggerOnMessageTable())
        {
            return true;
        } else {
            qDebug() << "Не удалось создать одну из таблиц";
            return false;
        }
    }
    else
    {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
}

/* Метод для открытия базы данных
 * */
bool DataBase::openDataBase()
{
    /* База данных открывается по заданному пути
     * и имени базы данных, если она существует
     * */
    db = QSqlDatabase::addDatabase("QSQLITE");//QSQLITE
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
bool DataBase::createUsersTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " USERS_TABLE  " ("
                            USER_ID " INTEGER PRIMARY KEY AUTOINCREMENT, "
                            USER_NICKNAME     " VARCHAR   NOT NULL,"
                            USER_PASSWORD     " VARCHAR   NOT NULL,"
                            USER_MAIL         " VARCHAR   NOT NULL,"
                            USER_NAME         " VARCHAR ,"
                            USER_SURNAME      " VARCHAR ,"
                            USER_IMAGE        " BLOB ,"
                            USER_DESCRIPTION  " VARCAHR ,"
                            USER_PHONE        " VARCHAR ,"
                            "UNIQUE (" USER_NICKNAME "," USER_MAIL "," USER_PHONE ")"
                             " )"
                    )){
        qDebug() << "DataBase: error of create " <<USERS_TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::createChatGroupsTable()
{
    QSqlQuery query;
    QString str=QString("CREATE TABLE " CHAT_TABLE " ("
                                                 " " CHAT_ID " INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                 " " CHAT_NAME " VARCHAR NOT NULL,"
                                                 " " CHAT_USER_CREATE_ID " INTEGER NOT NULL, "
                                                 "FOREIGN KEY (" CHAT_USER_CREATE_ID ") REFERENCES " USERS_TABLE " (" USER_ID ") ON DELETE CASCADE )");
    qDebug()<< "CREATE DATABASE " CHAT_TABLE "";
    if(query.exec(str))
        return true;
    else
    {
        qDebug()<<query.lastError();
        return false;
    }
}

bool DataBase::createNewChat(const QString &name_user_create, const QString &chatname)
{
    QSqlQuery query;
    QString str=QString("INSERT INTO " CHAT_TABLE " "
                                                        "(" CHAT_NAME ", " CHAT_USER_CREATE_ID ")"
                                                        "VALUES("
                                                        "('"+chatname+"'), "
                                                        "(SELECT " USER_ID " FROM " USERS_TABLE " WHERE " USER_ID " = '"+name_user_create+"') "//дописать
                                                        " ");
    qDebug()<< "try insert into table- "<<CHAT_TABLE<<" this chatname= "<<chatname;
    if(query.exec(str))
        return true;
    else
    {
        qDebug()<<query.lastError();
        return false;
    }
}

bool DataBase::createGroupParticipantsTable()
{
    QSqlQuery query;
    QString str=QString("CREATE TABLE " PARTICIPANTS_TABLE " ("
                                                 " " CHAT_ID " INTEGER NOT NULL,"
                                                 " " USER_ID " INTEGER NOT NULL,"
                                                 "FOREIGN KEY (" CHAT_ID ") REFERENCES " CHAT_TABLE " (" CHAT_ID ") ON DELETE CASCADE "
                                                 "FOREIGN KEY (" USER_ID ") REFERENCES " USERS_TABLE " (" USER_ID ") ON DELETE CASCADE )"  );
    qDebug()<< "CREATE DATABASE " PARTICIPANTS_TABLE "";
    if(query.exec(str))
        return true;
    else
    {
        qDebug()<<query.lastError();
        return false;
    }
}

bool DataBase::createNewGroupParticipants(const QString chatname, const QString &userID)
{
    QSqlQuery query;
    QString str=QString("INSERT INTO " PARTICIPANTS_TABLE " "
                                                        "(" CHAT_ID ", " USER_ID ")"
                                                        "VALUES("
                                                        "(SELECT " CHAT_ID " FROM " CHAT_TABLE " WHERE  " CHAT_NAME " = '"+chatname+"' ), "
                                                        "(SELECT " USER_ID " FROM " USERS_TABLE " WHERE " USER_ID " = '"+userID+"') "//дописать
                                                        " ");
    qDebug()<< "try insert into table- "<<PARTICIPANTS_TABLE<<" this chatname= "<<chatname;
    if(query.exec(str))
        return true;
    else
    {
        qDebug()<<query.lastError();
        return false;
    }
}

bool DataBase::createArchiveMessageTable()
{
    QSqlQuery query;
    QString str=QString("CREATE TABLE " MESSAGE_TABLE " ("
                                                 " " MESSAGE_ID " INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                 " " MESSAGE_TOID " INTEGER NOT NULL,"
                                                 " " MESSAGE_FROMID " INTEGER NOT NULL,"
                                                 " " CHAT_ID " INTEGER ,"
                                                 " " MESSAGE_CONTENT " VARCHAR ,"
                                                 " " MESSAGE_DATA " VARCHAR NOT NULL, "
                                                 "FOREIGN KEY (" MESSAGE_TOID ") REFERENCES " USERS_TABLE " (" USER_ID ") ON DELETE CASCADE "
                                                 "FOREIGN KEY (" MESSAGE_FROMID ") REFERENCES " USERS_TABLE " (" USER_ID ") ON DELETE CASCADE "
                                                 "FOREIGN KEY (" CHAT_ID ") REFERENCES " CHAT_TABLE " (" CHAT_ID ") ON DELETE CASCADE )" );
    qDebug()<< "CREATE DATABASE " MESSAGE_TABLE "";
    if(query.exec(str))
        return true;
    else
    {
        qDebug()<<query.lastError();
        return false;
    }
}

bool DataBase::createStatusMessageTable()
{
    QSqlQuery query;
    QString str=QString("CREATE TABLE " STATUS_MESS_TABLE " ("
                                                 " " MESSAGE_ID " INTEGER NOT NULL,"
                                                 " " USER_ID " INTEGER NOT NULL,"
                                                 " " STATUS_MESSAGE " VARCHAR NOT NULL,"
                                                 "FOREIGN KEY (" MESSAGE_ID ") REFERENCES " MESSAGE_TABLE " (" MESSAGE_ID ") ON DELETE CASCADE "
                                                 "FOREIGN KEY (" USER_ID ") REFERENCES " USERS_TABLE " (" USER_ID ") ON DELETE CASCADE )" );
    qDebug()<< "CREATE DATABASE " STATUS_MESS_TABLE "";
    if(query.exec(str))
        return true;
    else
    {
        qDebug()<<query.lastError();
        return false;
    }
}

bool DataBase::createTriggerOnMessageTable()
{
    QSqlQuery query;
    QString str=QString("CREATE TRIGGER add_status_message AFTER INSERT"
                        " ON " MESSAGE_TABLE " BEGIN"
                        " INSERT INTO " STATUS_MESS_TABLE " "
                        " (" MESSAGE_ID ", " USER_ID ", " STATUS_MESSAGE ")"
                        " VALUES (NEW." MESSAGE_ID ", NEW." MESSAGE_FROMID ", 'send'),"
                        " (NEW." MESSAGE_ID ", NEW." MESSAGE_TOID ", 'notread');"
                        " END; ");
    qDebug()<< "CREATE TRIGGER ON " MESSAGE_TABLE "";
    if(query.exec(str))
        return true;
    else
    {
        qDebug()<<query.lastError();
        return false;
    }
}


bool DataBase::insertIntoMessageTable(const QString &user_from,const QString &user_to, const QString &message, const QString &data)
{
    QSqlQuery query;
    QString str=QString("INSERT INTO " MESSAGE_TABLE " "
                                                        "(" MESSAGE_FROMID ", " MESSAGE_TOID ", " MESSAGE_CONTENT ", " MESSAGE_DATA ")"
                                                        "VALUES("
                                                        "(SELECT " USER_ID " FROM " USERS_TABLE " WHERE " USER_NICKNAME " = '"+user_from+"'),"//дописать
                                                        "(SELECT " USER_ID " FROM " USERS_TABLE " WHERE " USER_NICKNAME " = '"+user_to+"'),"
                                                        "('"+message+"'),"
                                                        "('"+data+"')"
                                                        ") ");
    qDebug()<< "try insert into table- "<<MESSAGE_TABLE<<" this meesage= "<<message;
    if(query.exec(str))
        return true;
    else
    {
        qDebug()<<query.lastError();
        return false;
    }
}

QVariantList DataBase::loadNewMessages(const QString &last_date, const QString &user_to_load)
{
    QSqlQuery query;
    QVariantList malesDataStatusFromImage;

    QString str = QString("SELECT " MESSAGE_TABLE "." MESSAGE_CONTENT ","
                          " " MESSAGE_TABLE "." MESSAGE_DATA ", "
                          " " STATUS_MESS_TABLE "." STATUS_MESSAGE ", "
                          " " USERS_TABLE "." USER_NICKNAME ", "
                          " " USERS_TABLE "." USER_IMAGE " "
                          " FROM " MESSAGE_TABLE " "
                          " JOIN " STATUS_MESS_TABLE "," USERS_TABLE " "
                          " ON " STATUS_MESS_TABLE "." MESSAGE_ID " = " MESSAGE_TABLE "." MESSAGE_ID " "
                          " AND " STATUS_MESS_TABLE "." USER_ID " =(SELECT " USER_ID " FROM " USERS_TABLE " "
                          " WHERE " USER_NICKNAME "='"+user_to_load+"' ) "
                          " AND " MESSAGE_TABLE "." MESSAGE_DATA " >'"+last_date+"' "
                          " AND " USERS_TABLE "." USER_NICKNAME "=(SELECT " USER_NICKNAME " "
                          "FROM " USERS_TABLE " WHERE " USER_ID "= " MESSAGE_TABLE "." MESSAGE_FROMID " ) "
                          " ORDER BY " MESSAGE_TABLE "." MESSAGE_DATA "; ");
          query.prepare(str);
          query.exec();

         if(query.isActive()){
             qDebug()<<"query is active now";
             while(query.next())
             {
                 malesDataStatusFromImage.append(query.value(0).toString());
                 malesDataStatusFromImage.append(query.value(1).toString());
                 malesDataStatusFromImage.append(query.value(2).toString());
                 malesDataStatusFromImage.append(query.value(3).toString());
                 malesDataStatusFromImage.append(query.value(4).toByteArray());
             }
         }
         else
             qDebug()<<"ERROR:"<<query.lastError();

         return malesDataStatusFromImage;
}


//дописать условия на референс в таблицу по имени пользователя!!!!..может юыть уже дописано
QStringList DataBase::loadArchiveMessages(const QString &user_from,const QString &user_to)
{
  QSqlQuery query;
  QStringList malesDataStatusFromTo;



  QString str = QString("SELECT " MESSAGE_TABLE "." MESSAGE_CONTENT ","
                        " " MESSAGE_TABLE "." MESSAGE_DATA ", "
                        " " STATUS_MESS_TABLE "." STATUS_MESSAGE ", "
                        "(SELECT " USERS_TABLE "." USER_NICKNAME " FROM " USERS_TABLE " "
                        " WHERE " USER_ID " = " MESSAGE_TABLE "." MESSAGE_FROMID " ) AS '_from' ,"
                        "(SELECT " USERS_TABLE "." USER_NICKNAME " FROM " USERS_TABLE " "
                        " WHERE " USER_ID " = " MESSAGE_TABLE "." MESSAGE_TOID " ) AS '_to'"
                        " FROM " MESSAGE_TABLE " "
                        " JOIN " STATUS_MESS_TABLE " "
                        " ON " STATUS_MESS_TABLE "." MESSAGE_ID " = " MESSAGE_TABLE "." MESSAGE_ID " "
                        " AND " STATUS_MESS_TABLE "." USER_ID " = " MESSAGE_TABLE "." MESSAGE_TOID " "
                        " WHERE " MESSAGE_TABLE "." MESSAGE_FROMID " = (SELECT " USER_ID " FROM " USERS_TABLE " WHERE " USER_NICKNAME " = '"+user_from+"') "
                        " AND " MESSAGE_TABLE "." MESSAGE_TOID " = (SELECT " USER_ID " FROM " USERS_TABLE " WHERE " USER_NICKNAME " = '"+user_to+"') "
                        " OR " MESSAGE_TABLE "." MESSAGE_FROMID " = (SELECT " USER_ID " FROM " USERS_TABLE " WHERE " USER_NICKNAME " = '"+user_to+"') "
                        " AND " MESSAGE_TABLE "." MESSAGE_TOID " = (SELECT " USER_ID " FROM " USERS_TABLE " WHERE " USER_NICKNAME " = '"+user_from+"') "
                        " ORDER BY " MESSAGE_TABLE "." MESSAGE_DATA "; ");
        query.prepare(str);
        query.exec();

       if(query.isActive()){
           qDebug()<<"query is active now";
           while(query.next())
           {
               malesDataStatusFromTo.append(query.value(0).toString());
               malesDataStatusFromTo.append(query.value(1).toString());
               malesDataStatusFromTo.append(query.value(2).toString());
               malesDataStatusFromTo.append(query.value(3).toString());
               malesDataStatusFromTo.append(query.value(4).toString());
           }
       }
       else
           qDebug()<<"ERROR:"<<query.lastError();

       return malesDataStatusFromTo;

}

QByteArray DataBase::getImageUser(const QString &name_user_to_load)
{
    QByteArray image;

    QSqlQuery query;
    QString str = QString("SELECT " USER_IMAGE " FROM " USERS_TABLE " WHERE " USER_NICKNAME " = '"+name_user_to_load+"'  ");
    query.prepare(str);
    query.exec();

    if(query.first())
        image=query.value(0).toByteArray();
    else
        qDebug()<<query.lastError();

    return image;
}

bool DataBase::setImageUser(const QString &name_user_to_load, const QByteArray &inByteArray)
{
    QSqlQuery query;

    query.prepare(" UPDATE " USERS_TABLE " SET " USER_IMAGE " = :IMAGESET "
                                            "WHERE " USER_NICKNAME " = :NICKNAMESET ");
    query.bindValue(":IMAGESET",  inByteArray);
    query.bindValue(":NICKNAMESET",  name_user_to_load);

    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert image in to " << USERS_TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;

}

bool DataBase::updateStatusMessageTable(const QString user, const QString status)
{
    qDebug()<<"TRY TO update status for user"<<user;
    QSqlQuery query;
    query.prepare(" UPDATE  " STATUS_MESS_TABLE " SET " STATUS_MESSAGE " = '"+status+"' "
                                            "WHERE " USER_ID " ="
                                            "(SELECT " USER_ID " FROM " USERS_TABLE " WHERE "
                                            " " USER_NICKNAME " =  '"+user+"' ) "
                                            " AND " STATUS_MESSAGE " !='send' ");

    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error UPDATE INFO in " << STATUS_MESS_TABLE<<"to user:"<<user;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

QString DataBase::getStatusMessage(const QString mess_id, const QString user)
{

}

QVariantList DataBase::getUsersBySearchText(const QString text_search)
{
    qDebug()<<"try get usersBySearch";
    QVariantList usernameAndImage;
    QString str="%"+text_search+"%";
    qDebug()<<"cтрока поиска"<<str;
    QSqlQuery query;
    query.prepare("SELECT " USER_NICKNAME "," USER_IMAGE " FROM " USERS_TABLE " WHERE " USER_NICKNAME " like :SEARCH  ");
    query.bindValue(":SEARCH",str);

    query.exec();

    if(query.isActive())
    {
        qDebug()<<"query searchUsers is active now";
        while(query.next())
        {
            usernameAndImage.append(query.value(0));
            usernameAndImage.append(query.value(1));
        }
    }
    else
        qDebug()<<"Error searchUsers"<<query.lastError();

    return usernameAndImage;
}
//переписать
QVariantList DataBase::getPersonalInfo(const QString &name_user_to_load)
{
    qDebug()<<"try get personInfo";
    QVariantList nameSurnameDescImage;

    QSqlQuery query;
    QString str = QString("SELECT " USER_NAME ","
                          " " USER_SURNAME ","
                          " " USER_DESCRIPTION ","
                          " " USER_IMAGE " FROM " USERS_TABLE " "
                          "WHERE " USER_NICKNAME " = '"+name_user_to_load+"'  ");
    query.prepare(str);
    query.exec();

    if(query.isActive())
        while(query.next())
        {
            nameSurnameDescImage.append(query.value(0));
            nameSurnameDescImage.append(query.value(1));
            nameSurnameDescImage.append(query.value(2));
            nameSurnameDescImage.append(query.value(3));
        }
    else
        qDebug()<<query.lastError();

    return nameSurnameDescImage;

}

bool DataBase::insertIntoUsersTable(const QString &nickname, const QString &password, const QString &mail, const QByteArray &shablonImage, const QString &phone)
{
    QSqlQuery query;
    /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
    query.prepare("INSERT INTO " USERS_TABLE " ( " USER_NICKNAME ", "
                                              USER_PASSWORD ","
                                              USER_MAIL ","
                                              USER_PHONE ","
                                              USER_IMAGE " ) "
                  "VALUES (:NICKNAME, :PASSWORD, :MAIL, :PHONE, :IMAGE )");
    query.bindValue(":NICKNAME",  nickname);
    query.bindValue(":PASSWORD",  password);
    query.bindValue(":MAIL",  mail);
    query.bindValue(":PHONE",  phone);
    query.bindValue(":IMAGE",  shablonImage);
    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert into " << USERS_TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::updatePersonalInfoUsersTable(const QString &name_user,const QString &name_add,const QString &surname_add,const QString &about)
{
    QSqlQuery query;
    query.prepare(" UPDATE  " USERS_TABLE " SET " USER_NAME " = '"+name_add+"' ,"
                                            USER_SURNAME " = '"+surname_add+"' ,"
                                            USER_DESCRIPTION " = '"+about+"'   "
                                            "WHERE " USER_NICKNAME " = '"+name_user+"' ");

    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error UPDATE INFO in " << USERS_TABLE<<"to user:"<<name_user;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::UpdatePasswordIntoMainTable(const QString &name_user,const QString &newpass)
{
    QSqlQuery query;
    query.prepare("UPDATE  " USERS_TABLE " SET " USER_PASSWORD" = '"+newpass+"' "
                                   "WHERE " USER_NICKNAME " = '"+name_user+"' ");

    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error UPDATE PASS in " << USERS_TABLE<<"to user:"<<name_user;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::check_name_exist(const QString &nickname)
{
    QSqlQuery query;
    QString names=nullptr;//для проверки совпадения логина
    QString str = QString("SELECT * FROM " USERS_TABLE " WHERE EXISTS "
                          "(SELECT " USER_NICKNAME " FROM " USERS_TABLE " WHERE " USER_NICKNAME "= '"+nickname+"')");

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
    
}

bool DataBase::check_log_In(const QString &nickname, const QString &password)
{
    QSqlQuery query;
    QString names=nullptr;//проверка наличия логина и пароля
    QString str = QString("SELECT * FROM " USERS_TABLE " WHERE EXISTS "
                          "(SELECT " USER_NICKNAME " FROM " USERS_TABLE " WHERE " USER_NICKNAME "= '"+nickname+"'"
                          "AND " USER_PASSWORD "= '"+password+"' )");

    query.prepare(str);
    query.exec();
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
}
