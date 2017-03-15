#ifndef IGAPI_H
#define IGAPI_H

#include <QObject>
#include "RequestManager.h"

struct IgAccount {
    QString username;
    QString password;
    QString igDataPath;
};

class IgApi : public QObject {
public:
    IgApi(const IgAccount &, QObject* = 0);

    void login();
    RequestReply getHashtagFeed(const QString &hashtagString, const QString &maxid = QString());
    RequestReply getLikedMedia();
    void like(const QString &mediaId);
    void unlike(const QString &mediaId);

    RequestReply GetMedia(const QString &url);

private:
    RequestReply syncFeatures();
    RequestReply autoCompleteUserList();
    RequestReply timelineFeed();
    RequestReply getv2Inbox();
    RequestReply getRecentActivity();

    QString generateSignature(QString);
    QString generateUUID(bool type);
    QString generateDeviceId();

    void CheckReplyStatus(const RequestReply &);

    QString username;
    QString password;

    QString uuid;
    QString device_id;
    QString username_id;
    QString token;
    bool isLoggedIn;
    QString rank_token;
    QString IGDataPath;

    RequestManager *rm;
};

#endif // IgApi_H
