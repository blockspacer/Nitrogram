#include "IgApiUtil.h"

#include <QFile>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>

void IgApiUtil::file_put_contents(QString filename, QString data) {
    QFile f(filename);

    if(!f.open(QIODevice::WriteOnly)) {
        throw QString("Cannot open file \"" + filename + "\"");
    }

    f.write(data.toUtf8());

    f.close();
}
QByteArray IgApiUtil::sha256_hmac(QByteArray stringToSign, QByteArray secretKey){
    QCryptographicHash::Algorithm alg = QCryptographicHash::Sha256;
    int blockSize = 64; // HMAC-SHA-1 & SHA-256 Blocksize

    if (secretKey.length() > blockSize) {
        secretKey = QCryptographicHash::hash(secretKey, alg);
    }

    if (secretKey.length() < blockSize) {
        int padding = blockSize - secretKey.length();
        secretKey.append(QByteArray(padding, char(0x00)));
    }
    QByteArray innerPadding(blockSize, char(0x36));
    QByteArray outerPadding(blockSize, char(0x5c));

    for (int i = 0; i < secretKey.length(); i++) {
        innerPadding[i] = innerPadding[i] ^ secretKey.at(i);
        outerPadding[i] = outerPadding[i] ^ secretKey.at(i);
    }

    QByteArray total = outerPadding;
    QByteArray part = innerPadding;
    part.append(stringToSign);
    total.append(QCryptographicHash::hash(part, alg));
    QByteArray hashed = QCryptographicHash::hash(total, alg);

    return hashed;
}
QString IgApiUtil::json_encode(const QVariantMap &data) {
    QString ret = QJsonDocument(QJsonObject::fromVariantMap(data)).toJson(QJsonDocument::Compact);
    return ret;
}
quint16 IgApiUtil::mt_rand(quint16 min, quint16 max) {
    return ((min + qrand()) % (max + 1));
}
QString IgApiUtil::file_get_contents(QString fname) {
    QString ret = "";

    QFile f(fname);
    if(f.open(QIODevice::ReadOnly)) {
        ret = f.readAll();
        f.close();
    }

    return ret;
}
