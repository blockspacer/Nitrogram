#include "IgApi.h"
#include "IgConst.h"
#include "IgApiUtil.h"

#include "JsonWalker.h"

#include <QFile>
#include <QTime>

#include <QDebug>

#include <QNetworkReply>

#include <QRegExp>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QCryptographicHash>

#define COOKIES_FILE_FOOTER     "-cookies.dat"
#define USERID_FILE_FOOTER      "-userId.dat"
#define TOKEN_FILE_FOOTER       "-token.dat"

IgApi::IgApi(const IgAccount &igAccount, QObject *parent) :
    QObject(parent)
{
    qsrand(QTime::currentTime().msecsSinceStartOfDay());

    this->isLoggedIn = false;

    this->username = igAccount.username;
    this->password = igAccount.password;
    this->IGDataPath = igAccount.igDataPath;

    QRegExp rx("[/\\]$");
    if(-1 == rx.indexIn(this->IGDataPath)) {
        this->IGDataPath += "\\";
    }

    this->uuid = this->generateUUID(true);
    this->device_id = this->generateDeviceId();

    if(     (QFile::exists(this->IGDataPath + this->username + COOKIES_FILE_FOOTER))
        &&  (QFile::exists(this->IGDataPath + this->username + USERID_FILE_FOOTER))
        &&  (QFile::exists(this->IGDataPath + this->username + TOKEN_FILE_FOOTER)) )
    {
        this->isLoggedIn = true;
        this->username_id = IgApiUtil::file_get_contents(this->IGDataPath + this->username + USERID_FILE_FOOTER).trimmed();
        this->rank_token = this->username_id + "_" + this->uuid;
        this->token = IgApiUtil::file_get_contents(this->IGDataPath + this->username + TOKEN_FILE_FOOTER).trimmed();
    }

    rm = new RequestManager(this->IGDataPath + this->username + COOKIES_FILE_FOOTER, this);
}
void IgApi::login() {
    qDebug() << "Check login...";

    if (!this->isLoggedIn) {
        qDebug() << "No login yet.";

        RequestReply reply;
        reply = rm->Get("si/fetch_headers/?challenge_type=signup&guid=" + this->generateUUID(false));

        QVariantMap data;
        data["device_id"]           = this->device_id;
        data["guid"]                = this->uuid;
        data["phone_id"]            = this->generateUUID(true);
        data["username"]            = this->username;
        data["password"]            = this->password;
        data["login_attempt_count"] = "0";

        reply = rm->Post("accounts/login/", this->generateSignature(IgApiUtil::json_encode(data)));

        CheckReplyStatus(reply);

        this->isLoggedIn = true;
        this->username_id = QString("%1").arg(JsonWalker::FindHierarchically("logged_in_user:pk", reply.data()).toDouble(), 0, 'f', 0);

        IgApiUtil::file_put_contents(this->IGDataPath + this->username + USERID_FILE_FOOTER, this->username_id);
        this->rank_token = this->username_id + "_" + this->uuid;

        QRegExp rx("csrftoken=([^;]+)");
        if(-1 == rx.indexIn(JsonWalker::FindString("Set-Cookie", reply.header()))) {
            throw QString("No token pattern. Exit");
        }
        this->token = rx.cap(1);

        IgApiUtil::file_put_contents(this->IGDataPath + this->username + TOKEN_FILE_FOOTER, this->token);

        this->syncFeatures();
        this->autoCompleteUserList();
        this->timelineFeed();
        this->getv2Inbox();
        this->getRecentActivity();

        return;
    }

    qDebug() << "Already logged in!";

    this->timelineFeed();
    this->getv2Inbox();
    this->getRecentActivity();
}
RequestReply IgApi::syncFeatures() {
    qDebug() << "syncFeatures";

    QVariantMap data;
    data["_uuid"]       = this->uuid;
    data["_uid"]        = this->username_id;
    data["id"]          = this->username_id;
    data["_csrftoken"]  = this->token;
    data["experiments"] = IgConst::experiments;

    RequestReply reply = rm->Post("qe/sync/", this->generateSignature(IgApiUtil::json_encode(data)));

    CheckReplyStatus(reply);

    return reply;
}
RequestReply IgApi::autoCompleteUserList() {
    qDebug() << "autoCompleteUserList";

    RequestReply reply = rm->Get("friendships/autocomplete_user_list/");

    CheckReplyStatus(reply);

    return reply;
}
RequestReply IgApi::timelineFeed() {
    qDebug() << "timelineFeed";

    RequestReply reply = rm->Get("feed/timeline/");

    CheckReplyStatus(reply);

    return reply;
}
RequestReply IgApi::getv2Inbox() {
    qDebug() << "getv2Inbox";

    RequestReply reply = rm->Get("direct_v2/inbox/?");

    CheckReplyStatus(reply);

    return reply;
}
RequestReply IgApi::getRecentActivity() {
    qDebug() << "getRecentActivity";

    RequestReply reply = rm->Get("news/inbox/?");

    CheckReplyStatus(reply);

    return reply;
}
RequestReply IgApi::getHashtagFeed(const QString &hashtagString, const QString &maxid) {
    qDebug() << "getHashtagFeed";

    QString endpoint;
    if(maxid.isNull()) {
        endpoint = QString("feed/tag/%1/?rank_token=%2&ranked_content=true&")
                .arg(hashtagString)
                .arg(this->rank_token);
    } else {
        endpoint = QString("feed/tag/%1/?max_id=%2&rank_token=%3&ranked_content=true&")
                .arg(hashtagString)
                .arg(maxid)
                .arg(this->rank_token);
    }

    RequestReply reply = rm->Get(endpoint);

    CheckReplyStatus(reply);

    return reply;
}
RequestReply IgApi::getLikedMedia() {
    qDebug() << "getLikedMedia";

    RequestReply reply = rm->Get("feed/liked/?");

    CheckReplyStatus(reply);

    return reply;
}
void IgApi::like(const QString &mediaId) {
    qDebug() << "like";

    QVariantMap data;
    data["_uuid"]       = this->uuid;
    data["_uid"]        = this->username_id;
    data["_csrftoken"]  = this->token;
    data["media_id"]    = mediaId;

    RequestReply reply = rm->Post("media/" + mediaId + "/like/",
                                  this->generateSignature(IgApiUtil::json_encode(data)));

    CheckReplyStatus(reply);
}
void IgApi::unlike(const QString &mediaId) {
    qDebug() << "unlike";

    QVariantMap data;
    data["_uuid"]       = this->uuid;
    data["_uid"]        = this->username_id;
    data["_csrftoken"]  = this->token;
    data["media_id"]    = mediaId;

    RequestReply reply = rm->Post("media/" + mediaId + "/unlike/",
                                  this->generateSignature(IgApiUtil::json_encode(data)));

    CheckReplyStatus(reply);
}
RequestReply IgApi::GetMedia(const QString &url) {
    qDebug() << "GetMedia";
    return rm->GetClear(url);
}

QString IgApi::generateSignature(QString data) {
    QString ret;

    QByteArray hash = IgApiUtil::sha256_hmac(data.toUtf8(), IgConst::igSigKey);

    ret = "ig_sig_key_version=";
    ret += IgConst::sigKeyVersion;
    ret += "&signed_body=";
    ret += hash.toHex();
    ret += ".";
    ret += QUrl::toPercentEncoding(data);

    return ret;
}
QString IgApi::generateDeviceId() {
    return QString::asprintf("android-%04x%04x%04x%04x",
                             IgApiUtil::mt_rand(0, 0xffff),
                             IgApiUtil::mt_rand(0, 0xffff),
                             IgApiUtil::mt_rand(0, 0xffff),
                             IgApiUtil::mt_rand(0, 0xffff));
}
QString IgApi::generateUUID(bool type) {
    QString ret = QString::asprintf("%04x%04x-%04x-%04x-%04x-%04x%04x%04x",
          IgApiUtil::mt_rand(0, 0xffff), IgApiUtil::mt_rand(0, 0xffff),
          IgApiUtil::mt_rand(0, 0xffff),
          IgApiUtil::mt_rand(0, 0x0fff) | 0x4000,
          IgApiUtil::mt_rand(0, 0x3fff) | 0x8000,
          IgApiUtil::mt_rand(0, 0xffff), IgApiUtil::mt_rand(0, 0xffff), IgApiUtil::mt_rand(0, 0xffff)
        );

    if(!type)
        ret.replace("-", "");

    return ret;
}
void IgApi::CheckReplyStatus(const RequestReply &reply) {
    if("ok" != JsonWalker::FindString("status", reply.data())) {
        throw JsonWalker::FindString("message", reply.data());
    }
}
