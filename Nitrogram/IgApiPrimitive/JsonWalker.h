#ifndef JSONWALKER_H
#define JSONWALKER_H

#include <QString>

#include <QJsonValue>
#include <QJsonObject>

namespace JsonWalker {
    QString FindString(const QString &, const QJsonObject &);
    QJsonValue FindHierarchically(const QString &, QJsonObject);
}

#endif // JSONWALKER_H
