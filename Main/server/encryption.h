#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <qrsaencryption.h>

#include <QCryptographicHash>
#include "qaesencryption.h"

class Encryption
{
public:
    Encryption();

    QByteArray encodeMessage(QString mess);
    QString decodeMessage(QByteArray key, QByteArray IV, QByteArray mess);
    QByteArray getHashKey();
    QByteArray getHashIV();

private:
    QRSAEncryption *rsaSecret;

    QAESEncryption *aesSecret;

    QByteArray hashKey;

    QByteArray hashIV;

    QString key;

    QString iv;


};

#endif // ENCRYPTION_H
