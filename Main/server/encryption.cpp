#include "encryption.h"
Encryption::Encryption()
{
    rsaSecret=new QRSAEncryption(QRSAEncryption::Rsa::RSA_2048);
    aesSecret=new QAESEncryption(QAESEncryption::AES_256,QAESEncryption::CBC);

     key= "your-string-key";
     iv= "your-IV-vector";

     hashKey= QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
     hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);
}

QByteArray Encryption::encodeMessage(QString mess)
{
    QByteArray encodeText = aesSecret->encode(mess.toLocal8Bit(), hashKey, hashIV);

    return encodeText;
}

QString Encryption::decodeMessage(QByteArray key, QByteArray IV, QByteArray mess)
{

    QByteArray decodeText = aesSecret->decode(mess, key, IV);

    QString decodedString = QString(aesSecret->removePadding(decodeText));

    return decodedString;
}

QByteArray Encryption::getHashKey()
{
    return hashKey;
}

QByteArray Encryption::getHashIV()
{
    return hashIV;
}

