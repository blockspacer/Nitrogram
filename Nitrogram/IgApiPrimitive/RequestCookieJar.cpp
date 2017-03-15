#include "RequestCookieJar.h"

#include <QMap>
#include <QMutex>
#include <QMutexLocker>

#include <QSettings>
#include <QFile>

#include <QDateTime>
#include <QDataStream>
#include <QDebug>

///////////////////////////////////////////////////////////////////////////
struct MutexMapHolder {
    ~MutexMapHolder() {
        foreach(QMutex *curMutex, _map) {
            delete curMutex;
        }
    }

    QMap<QString, QMutex*> _map;
};
///////////////////////////////////////////////////////////////////////////
class MyMutexLocker {
public:
    MyMutexLocker(const QString &name) {
        QMutexLocker locker(&_mapWriteMutex);

        _name = name;

        LockMutex();
    }
    ~MyMutexLocker() {
        QMutexLocker locker(&_mapWriteMutex);

        UnlockMutex();
    }

private:
    void LockMutex() {
        QMutex *mutex = _mutexHolder._map.value(_name, 0);

        if(0 == mutex) {
            mutex = new QMutex();
            _mutexHolder._map.insert(_name, mutex);
        }

        mutex->lock();
    }
    void UnlockMutex() {
        QMutex *mutex = _mutexHolder._map.value(_name, 0);
        if(0 == mutex) {
            throw QString("!!!! NO mutex to unlock !!!!");
        }

        mutex->unlock();
    }

    static MutexMapHolder _mutexHolder;
    static QMutex _mapWriteMutex;

    QString _name;
};
MutexMapHolder MyMutexLocker::_mutexHolder;
QMutex MyMutexLocker::_mapWriteMutex;
///////////////////////////////////////////////////////////////////////////
QDataStream &operator<<(QDataStream &stream, const QList<QNetworkCookie> &list)
{
    //stream << JAR_VERSION;
    stream << quint32(list.size());
    for (int i = 0; i < list.size(); ++i)
        stream << list.at(i).toRawForm();
    return stream;
}
QDataStream &operator>>(QDataStream &stream, QList<QNetworkCookie> &list)
{
    list.clear();

    //quint32 version;
    //stream >> version;

    //if (version != JAR_VERSION)
    //    return stream;

    quint32 count;
    stream >> count;
    for(quint32 i = 0; i < count; ++i) {
        QByteArray value;
        stream >> value;
        QList<QNetworkCookie> newCookies = QNetworkCookie::parseCookies(value);
        if (newCookies.count() == 0 && value.length() != 0) {
            qWarning() << "CookieJar: Unable to parse saved cookie:" << value;
        }
        for (int j = 0; j < newCookies.count(); ++j)
            list.append(newCookies.at(j));
        if (stream.atEnd())
            break;
    }
    return stream;
}
///////////////////////////////////////////////////////////////////////////

RequestCookieJar::RequestCookieJar(QString name, QObject *parent) :
    QNetworkCookieJar(parent),
    _name(name)
{
    qRegisterMetaTypeStreamOperators<QList<QNetworkCookie> >("QList<QNetworkCookie>");
    LoadCookies();
}
RequestCookieJar::~RequestCookieJar() {

}
QList<QNetworkCookie> RequestCookieJar::cookiesForUrl(const QUrl &url) const {
    RequestCookieJar *that = const_cast<RequestCookieJar*>(this);

    that->LoadCookies();

    return QNetworkCookieJar::cookiesForUrl(url);
}
bool RequestCookieJar::deleteCookie(const QNetworkCookie &cookie) {
    LoadCookies();

    bool ret = QNetworkCookieJar::deleteCookie(cookie);

    FlushCookies();

    return ret;
}
bool RequestCookieJar::insertCookie(const QNetworkCookie &cookie) {
    LoadCookies();

    bool ret = QNetworkCookieJar::insertCookie(cookie);

    FlushCookies();

    return ret;
}
bool RequestCookieJar::setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url) {
    LoadCookies();

    bool ret = QNetworkCookieJar::setCookiesFromUrl(cookieList, url);

    FlushCookies();

    return ret;
}
bool RequestCookieJar::updateCookie(const QNetworkCookie &cookie) {
    LoadCookies();

    bool ret = QNetworkCookieJar::updateCookie(cookie);

    FlushCookies();

    return ret;
}
void RequestCookieJar::FlushCookies() {
    MyMutexLocker locker(_name);

    QSettings cookieSettings(_name, QSettings::IniFormat);

    QList<QNetworkCookie> cookies = allCookies();
    for (int i = cookies.count() - 1; i >= 0; --i) {
        if(cookies.at(i).isSessionCookie()) {
            cookies.removeAt(i);
            continue;
        }
        if(cookies.at(i).expirationDate() < QDateTime::currentDateTime()) {
            cookies.removeAt(i);
            continue;
        }
    }

    cookieSettings.setValue(QLatin1String("cookies"), QVariant::fromValue<QList<QNetworkCookie> >(cookies));
}
void RequestCookieJar::LoadCookies() {
    MyMutexLocker locker(_name);

    if(!QFile(_name).exists())
        return;

    QSettings cookieSettings(_name, QSettings::IniFormat);
    setAllCookies(qvariant_cast<QList<QNetworkCookie> >(cookieSettings.value(QLatin1String("cookies"))));
}
