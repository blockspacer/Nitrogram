#ifndef REQUESTREPLY_H
#define REQUESTREPLY_H

#include <QNetworkReply>
#include <QJsonObject>

class RequestReply {
public:
    RequestReply(const QList<QNetworkReply::RawHeaderPair>&, const QByteArray& = QByteArray());
    RequestReply(const RequestReply&);
    RequestReply();

    RequestReply& operator =(const RequestReply&);
    RequestReply& operator <<(const QList<QNetworkReply::RawHeaderPair>&);
    RequestReply& operator <<(const QByteArray&);

    QJsonObject& data();
    QJsonObject& header();
    const QJsonObject& data() const;
    const QJsonObject& header() const;
    const QByteArray& bin() const;

private:
    QJsonObject _data;
    QJsonObject _header;
    QByteArray _bin;
};

#endif
