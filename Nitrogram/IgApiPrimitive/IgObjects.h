#ifndef IGOBJECTS_H
#define IGOBJECTS_H

#include <QJsonObject>

#include <QList>
#include <QString>

class MediaItem {
public:
    MediaItem(const QJsonObject&);

    QString GetCaption() const;
    QString GetId() const;
    bool IsLiked() const;
    QString GetMaxImageUrl() const;
    QString GetMinImageUrl() const;

private:
    QJsonObject jo;
};

typedef QList<MediaItem> MediaItemList;

#endif // IGOBJECTS_H
