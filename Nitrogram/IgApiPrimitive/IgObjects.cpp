#include "IgObjects.h"

#include "JsonWalker.h"

#include <QJsonArray>

template<typename DecisionMaker>
QString GetImageUrl(int height, DecisionMaker dm, const QJsonObject &jo) {
    QJsonValue images = JsonWalker::FindHierarchically("image_versions2:candidates", jo);

    if(!images.isArray()) {
        throw QString("No image array!");
    }

    QJsonArray imgArray = images.toArray();
    QString url;

    foreach(const QJsonValue &curVal, imgArray) {
        if(!curVal.isObject()) {
            throw QString("Image candidate is not an object");
        }

        QJsonObject curJo = curVal.toObject();

        int curHeight = JsonWalker::FindHierarchically("height", curJo).toInt();
        if(!dm(curHeight, height)) {
            continue;
        }

        height = curHeight;
        url = JsonWalker::FindString("url", curJo);
    }

    return url;

}

MediaItem::MediaItem(const QJsonObject &_jo) {
    jo = _jo;
}
QString MediaItem::GetCaption() const {
    return JsonWalker::FindString("caption:text", jo);
}
QString MediaItem::GetId() const {
    return JsonWalker::FindString("id", jo);
}
bool MediaItem::IsLiked() const {
    return JsonWalker::FindHierarchically("has_liked", jo).toBool();
}
QString MediaItem::GetMaxImageUrl() const {
    int height = 0;
    return GetImageUrl(height, [](int a, int b) {
        return (a > b);
    }, jo);
}
QString MediaItem::GetMinImageUrl() const {
    int height = 0xffff;
    return GetImageUrl(height, [](int a, int b) {
        return (a < b);
    }, jo);
}
