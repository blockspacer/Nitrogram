#include "RequestManager.h"
#include "IgConst.h"

#include "RequestCookieJar.h"

#include <QMutex>
#include <QMutexLocker>

#include <QEventLoop>

namespace RequestManagerCounter {
    QMutex mutex;
    int counter = 0;

    int GetNext() {
        QMutexLocker locker(&mutex);

        return ++counter;
    }
}

#define LOG(a)     qDebug() << instNumber << ": |" #a "| "

RequestManager::RequestManager(const QString &cjPath, QObject *parent) :
    QObject(parent),
    nam(new QNetworkAccessManager(this)),
    instNumber(RequestManagerCounter::GetNext())
{
    nam->setCookieJar(new RequestCookieJar(cjPath, this));

    #define NAM_CONNECT(name)                             \
            connect(nam, &QNetworkAccessManager::name,    \
                this, &RequestManager::name);

    NAM_CONNECT(authenticationRequired);
    NAM_CONNECT(encrypted);
    NAM_CONNECT(finished);
    NAM_CONNECT(networkAccessibleChanged);
    NAM_CONNECT(preSharedKeyAuthenticationRequired);
    NAM_CONNECT(proxyAuthenticationRequired);
    NAM_CONNECT(sslErrors);
}
RequestReply RequestManager::PerfornRequest(const QUrl &url, const QByteArray &data, bool isPostRequest) {
    QNetworkRequest req;

    FillRequestFields(req, url);

    //LOG(REQ) << req.rawHeaderList();

    QNetworkReply *rep = isPostRequest ? nam->post(req, data) : nam->get(req);
    CreateReplyConnections(rep);

    QEventLoop loop;
    connect(rep, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    RequestReply ret = RequestReply() << rep->rawHeaderPairs() << rep->readAll();

    //LOG(GET) << ret.header();
    //LOG(GET) << ret.data();

    rep->disconnect();
    rep->deleteLater();

    return ret;
}
RequestReply RequestManager::GetClear(const QString &endpoint) {
    QUrl url = QUrl(endpoint);
    return PerfornRequest(url);
}
RequestReply RequestManager::Get(const QString &endpoint) {
    return GetClear(IgConst::apiUrl + endpoint);
}
RequestReply RequestManager::Post(const QString &endpoint, const QString &data) {
    QUrl url = QUrl(IgConst::apiUrl + endpoint);
    return PerfornRequest(url, data.toUtf8(), true);
}
void RequestManager::CreateReplyConnections(const QNetworkReply *rep) const {
    #define REP_CONNECT(name)                       \
        QObject::connect(rep, &QNetworkReply::name,          \
            this, &RequestManager::rep_ ## name);
    //*
    REP_CONNECT(downloadProgress);
    connect(rep, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
            this, &RequestManager::rep_error);
    REP_CONNECT(encrypted);
    REP_CONNECT(finished);
    REP_CONNECT(metaDataChanged);
    REP_CONNECT(preSharedKeyAuthenticationRequired);
    REP_CONNECT(redirected);
    REP_CONNECT(sslErrors);
    REP_CONNECT(uploadProgress);
    //*/
}
void RequestManager::FillRequestFields(QNetworkRequest &req, const QUrl &url) {
    req.setRawHeader("Connection", "close");
    req.setRawHeader("Accept", "*/*");
    req.setRawHeader("Content-type", "application/x-www-form-urlencoded; charset=UTF-8");
    req.setRawHeader("Cookie2", "$Version=1");
    req.setRawHeader("Accept-Language", "en-US");

    req.setUrl(url);

    req.setHeader(QNetworkRequest::UserAgentHeader, IgConst::userAgent);

    req.setMaximumRedirectsAllowed(20);
}
//------------------------ QNetworkReply signals ------------------------------
void RequestManager::rep_downloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    LOG(rep) << "repDownloadProgress";
}
void RequestManager::rep_encrypted() {
    LOG(rep) << "repEncrypted";
}
void RequestManager::rep_error(QNetworkReply::NetworkError code) {
    LOG(rep) << "repError";
}
void RequestManager::rep_finished() {
    LOG(rep) << "repFinished";
}
void RequestManager::rep_metaDataChanged() {
    LOG(rep) << "repMetaDataChanged";
}
void RequestManager::rep_preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator) {
    LOG(rep) << "repPreSharedKeyAuthenticationRequired";
}
void RequestManager::rep_redirected(const QUrl &url) {
    LOG(rep) << "repRedirected";
}
void RequestManager::rep_sslErrors(const QList<QSslError> &errors) {
    LOG(rep) << "repSslErrors";
}
void RequestManager::rep_uploadProgress(qint64 bytesSent, qint64 bytesTotal) {
    LOG(rep) << "repUploadProgress";
}
//---------------------- QNetworkAccessManager signals ------------------------
void RequestManager::authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator) {
    LOG(nam) << "authenticationRequired";
}
void RequestManager::encrypted(QNetworkReply *reply) {
    LOG(nam) << "encrypted";
}
void RequestManager::finished(QNetworkReply *reply) {
    LOG(nam) << "finished";
}
void RequestManager::networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible) {
    LOG(nam) << "networkAccessibleChanged";
}
void RequestManager::preSharedKeyAuthenticationRequired(QNetworkReply *reply, QSslPreSharedKeyAuthenticator *authenticator) {
    LOG(nam) << "preSharedKeyAuthenticationRequired";
}
void RequestManager::proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator) {
    LOG(nam) << "proxyAuthenticationRequired";
}
void RequestManager::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors) {
    LOG(nam) << "sslErrors";
}
