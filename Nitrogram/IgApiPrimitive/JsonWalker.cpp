#include <JsonWalker.h>

#include <QStringList>

QJsonValue JsonWalker::FindHierarchically(const QString &what, QJsonObject jo) {
    QJsonValue ret;

    QStringList keys = what.split(':');
    foreach(const QString &key, keys) {
        QJsonObject::const_iterator it = jo.find(key);
        if(it == jo.end()) {
            throw QString("No \"" + what + "\" field found!");
        }

        ret = it.value();
        jo = ret.toObject();
    }

    return ret;
}
QString JsonWalker::FindString(const QString &what, const QJsonObject &jo) {
    return FindHierarchically(what, jo).toString();
}
