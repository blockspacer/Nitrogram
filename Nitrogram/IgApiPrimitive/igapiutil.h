#ifndef IGAPIUTIL_H
#define IGAPIUTIL_H

#include <QString>
#include <QVariantMap>
#include <QByteArray>

namespace IgApiUtil {
    quint16 mt_rand(quint16 min, quint16 max);
    QString file_get_contents(QString fname);
    QString json_encode(const QVariantMap&);
    QByteArray sha256_hmac(QByteArray stringToSign, QByteArray secretKey);
    void file_put_contents(QString filename, QString data);
}

#endif // IGAPIUTIL_H
