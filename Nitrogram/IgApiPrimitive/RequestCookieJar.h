#ifndef REQUESTCOOKIEJAR_H
#define REQUESTCOOKIEJAR_H

#include <QNetworkCookie>
#include <QNetworkCookieJar>

#include <QString>

class RequestCookieJar : public QNetworkCookieJar {
public:
    RequestCookieJar(QString name, QObject *parent = Q_NULLPTR);
    ~RequestCookieJar();

    QList<QNetworkCookie> cookiesForUrl(const QUrl &url) const;
    bool deleteCookie(const QNetworkCookie &cookie);
    bool insertCookie(const QNetworkCookie &cookie);
    bool setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url);
    bool updateCookie(const QNetworkCookie &cookie);

private:
    void FlushCookies();
    void LoadCookies();

    QString _name;
};

#endif
