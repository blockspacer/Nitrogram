#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QSslPreSharedKeyAuthenticator>
#include <QNetworkProxy>

#include <QList>
#include <QSslError>

#include "RequestReply.h"

class RequestManager : public QObject {
    Q_OBJECT

public:
    explicit RequestManager(const QString &cjPath, QObject *parent = 0);

    RequestReply Get(const QString &endpoint);
    RequestReply GetClear(const QString &endpoint);
    RequestReply Post(const QString &endpoint, const QString &data);

signals:

private slots:
    void authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
    void encrypted(QNetworkReply *reply);
    void finished(QNetworkReply *reply);
    void networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible);
    void preSharedKeyAuthenticationRequired(QNetworkReply *reply, QSslPreSharedKeyAuthenticator *authenticator);
    void proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);

    void rep_downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void rep_encrypted();
    void rep_error(QNetworkReply::NetworkError code);
    void rep_finished();
    void rep_metaDataChanged();
    void rep_preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator);
    void rep_redirected(const QUrl &url);
    void rep_sslErrors(const QList<QSslError> &errors);
    void rep_uploadProgress(qint64 bytesSent, qint64 bytesTotal);

private:
    RequestReply PerfornRequest(const QUrl &url, const QByteArray &data = QByteArray(), bool isPostRequest = false);
    void FillRequestFields(QNetworkRequest&, const QUrl&);
    void CreateReplyConnections(const QNetworkReply*) const;

    QNetworkAccessManager *nam;
    int instNumber;
};

#endif // REQUESTMANAGER_H
