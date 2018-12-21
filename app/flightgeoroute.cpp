#include "flightgeoroute.h"

#include <QtMath>
#include <QPointF>

#include "manoeuvre.h"

void FlightGeoRoute::addGeoPoint(const QGeoCoordinate &geoPoint)
{
    // Formally it's not correct to assume that
    // 'pixel' validation for manoeuvre is correct
    // but here we expect valid geo flight segment

    mGeoPoints << QVariant::fromValue(geoPoint);
    if (mGeoPath.isEmpty()) {
        mStart = geoPoint;
        mGeoPath << QVariant::fromValue(geoPoint);
        return;
    }

    createLastGeoSegment(geoPoint);
}

void FlightGeoRoute::createLastGeoSegment(const QGeoCoordinate &point)
{
    const int index = mGeoPoints.size() - 1;
    if (index == 1) {
        mMiddle = point;
        mGeoPath << QVariant::fromValue(point);
        return;
    }

    // In relative plane coordinate system mStart is 0
    const QPointF start(0, 0);
    const auto &middle = toPlaneCoordinate(mStart, mMiddle);
    const auto &finish = toPlaneCoordinate(mStart, point);

    // Assume that Boeing-747 has maximum speed 920km/h
    // G-force is 3g, so radius = v*v/g-force -> 21770m
    Manoeuvre manoeuvre(start, middle, finish, 21770);
    manoeuvre.toGeoManoeuvre();

    // Let's go over manoeuvre arc and find geo points
    auto arcAngle = 0.0;
    QGeoCoordinate arcGeoPoint;
    while (arcAngle < manoeuvre.spanAngle()) {
        auto sign = manoeuvre.isLeftTurn() ? 1 : -1;
        auto angle = manoeuvre.startAngle() - sign*arcAngle;
        auto radius = manoeuvre.radius();
        QPointF arcPoint(radius*qSin(angle), radius*qCos(angle));
        arcAngle += 0.01;

        arcGeoPoint = toGeoCoordinate(mStart, manoeuvre.circlePoint() + arcPoint);
        mGeoPath << QVariant::fromValue(arcGeoPoint);
    }

    // Prepare info for next manoeuvre
    mStart = arcGeoPoint;
    mMiddle = point;

    mGeoPath << QVariant::fromValue(point);
}

QPointF FlightGeoRoute::toPlaneCoordinate(const QGeoCoordinate &origin,
                                          const QGeoCoordinate &point)
{
    auto distance = origin.distanceTo(point);
    auto azimuth = origin.azimuthTo(point);

    auto x = qSin(qDegreesToRadians(azimuth)) * distance;
    auto y = qCos(qDegreesToRadians(azimuth)) * distance;

    return QPointF(x, y);
}

QGeoCoordinate FlightGeoRoute::toGeoCoordinate(const QGeoCoordinate &origin, const QPointF &point)
{
    auto distance = qSqrt(point.x()*point.x() + point.y()*point.y());
    auto radianAngle = qAtan2(point.x(), point.y());
    auto azimuth = qRadiansToDegrees(radianAngle < 0 ? radianAngle + 2*M_PI : radianAngle);

    return origin.atDistanceAndAzimuth(distance, azimuth);
}
