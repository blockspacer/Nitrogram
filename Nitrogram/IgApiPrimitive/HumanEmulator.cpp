#include "HumanEmulator.h"

#include "JsonWalker.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

HumanEmulator::HumanEmulator(const IgAccount &_account) {
    this->moveToThread(this);
    igAccount = _account;
}
MediaItemList HumanEmulator::ExtractItems(const RequestReply &reply) {
    MediaItemList ret;

    QJsonValue items = JsonWalker::FindHierarchically("items", reply.data());

    if(!items.isArray()) {
        throw QString("Items is not an array!");
    }

    QJsonArray itemsArray = items.toArray();
    for(QJsonArray::iterator it = itemsArray.begin(); it != itemsArray.end(); it++) {
        if(!it->isObject()) {
            throw QString("Item is not an JsonObject");
        }

        ret << MediaItem(it->toObject());
    }

    int expectedCount = JsonWalker::FindHierarchically("num_results", reply.data()).toInt();
    if(ret.count() != expectedCount) {
        throw QString("Received items count is unexpected!");
    }

    return ret;
}
void HumanEmulator::run() {
    try {
        igApi = new IgApi(igAccount, this);

        igApi->login();

        RequestReply reply;
        QString maxid = QString();
        int count = 2;
        do {
            reply = igApi->getHashtagFeed("guitar", maxid);

            MediaItemList items = ExtractItems(reply);

            qDebug() << "Found items " << items.count();
            for(int i = 0; i < items.count(); ++i) {
                qDebug() << i+1 << ": " << items.at(i).GetCaption().left(20);

                igApi->GetMedia(items.at(i).GetMinImageUrl());

                count = 1;
                //break;
            }

            igApi->GetMedia(items.at(0).GetMaxImageUrl());
            igApi->like(items.at(0).GetId());
            qDebug() << "Media id: " << items.at(0).GetId();


            bool moreAvailable = JsonWalker::FindHierarchically("more_available", reply.data()).toBool();
            if(!moreAvailable) {
                qDebug() << "No more avaliable";
                break;
            }

            maxid = JsonWalker::FindString("next_max_id", reply.data());

            sleep(4);
        }
        while(0 != --count);

        reply = igApi->getLikedMedia();
        MediaItemList items = ExtractItems(reply);
        qDebug() << "Found items " << items.count();
        foreach(const MediaItem &item, items) {
            igApi->unlike(item.GetId());
        }


        reply = igApi->getLikedMedia();
        //qDebug() << reply.header();
        qDebug() << QString(QJsonDocument(reply.data()).toJson());

        #ifndef QT_NO_DEBUG
        exec();
        #endif
    }
    catch(const QString &err) {
        qDebug() << err;
    }
    catch(...) {
        qDebug() << "Unknown exception!";
    }
}
