#include <QtTest>
#include <QCoreApplication>

#include <IgApi.h>

class IgApiTest : public QObject {
    Q_OBJECT

public:
    IgApiTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void Login();
    void GetHashtagFeed();

private:
    IgApi *api;
};

IgApiTest::IgApiTest() :
    api(0)
{
}

void IgApiTest::initTestCase() {
    api = new IgApi();
}
void IgApiTest::cleanupTestCase() {
    if(api) {
        delete api;
        api = 0;
    }
}

void IgApiTest::Login() {
    QVERIFY(api->Login());
}
void IgApiTest::GetHashtagFeed() {

}

QTEST_MAIN(IgApiTest)

#include "IgApiTest.moc"
