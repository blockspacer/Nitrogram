#include "RequestReply.h"

#include <QJsonDocument>

RequestReply::RequestReply(const RequestReply &a) {
    *this = a;
}
RequestReply::RequestReply() {
}
RequestReply& RequestReply::operator=(const RequestReply &a) {
    _data = a._data;
    _header = a._header;
    _bin = a._bin;

    return *this;
}
RequestReply& RequestReply::operator <<(const QList<QNetworkReply::RawHeaderPair> &hdr) {
    QJsonObject jo;

    foreach(const QNetworkReply::RawHeaderPair &cur, hdr) {
        jo.insert(QString(cur.first), QString(cur.second));
    }

    _header = jo;

    return *this;
}
RequestReply& RequestReply::operator <<(const QByteArray &data) {
    QJsonDocument jd = QJsonDocument::fromJson(data);
    if(!jd.isObject()) {
        qDebug() << "|RequestReply| input data is not a JSON object";
        _bin = data;
    }
    else {
        _data = jd.object();
    }

    return *this;
}
const QByteArray& RequestReply::bin() const {
    return _bin;
}
QJsonObject& RequestReply::data() {
    return _data;
}
QJsonObject& RequestReply::header() {
    return _header;
}
const QJsonObject& RequestReply::data() const {
    return _data;
}
const QJsonObject& RequestReply::header() const {
    return _header;
}
