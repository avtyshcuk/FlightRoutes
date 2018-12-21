#include <QString>
#include <QtTest>

#include "manoeuvre.h"
#include "manoeuvre.cpp"

class ManoeuvreTest : public QObject
{
    Q_OBJECT

public:
    ManoeuvreTest();

private Q_SLOTS:
    void manoeuvreTest();
};

ManoeuvreTest::ManoeuvreTest()
{
}

void ManoeuvreTest::manoeuvreTest()
{
    Manoeuvre samePointsManoeuvre(QPointF(0, 0), QPointF(0, 0), QPointF(0, 0), 10);
    QVERIFY2(samePointsManoeuvre.isValid(), "Failure for all 3 are same points");

    Manoeuvre xAxisPerpendManoeuvre(QPointF(0, 0), QPointF(0, 100), QPointF(200, 200), 10);
    QVERIFY2(xAxisPerpendManoeuvre.isValid(), "Failure when entering line is x-axis perpendicular");

    Manoeuvre yAxisPerpendManoeuvre(QPointF(0, 0), QPointF(100, 0), QPointF(200, 200), 10);
    QVERIFY2(yAxisPerpendManoeuvre.isValid(), "Failure when entering line is y-axis perpendicular");

    Manoeuvre oneLineManoeuvre(QPointF(0, 0), QPointF(100, 100), QPointF(200, 200), 10);
    QVERIFY2(oneLineManoeuvre.isValid(), "Failure when three points lie on the same line");

    Manoeuvre brokenManoeuvre(QPointF(0, 0), QPointF(100, 100), QPointF(101, 103), 10);
    QVERIFY2(!brokenManoeuvre.isValid(), "Failure when broken manoeuvre");

    Manoeuvre normalManoeuvre(QPointF(0, 0), QPointF(100, 100), QPointF(200, 300), 10);
    QVERIFY2(normalManoeuvre.isValid(), "Failure when normal manoeuvre");
}

QTEST_APPLESS_MAIN(ManoeuvreTest)

#include "tst_manoeuvretest.moc"
