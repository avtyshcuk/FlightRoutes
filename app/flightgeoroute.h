#ifndef FLIGHTGEOROUTE_H
#define FLIGHTGEOROUTE_H

#include <QGeoCoordinate>
#include <QVariantList>
#include <QGeoPath>

class FlightGeoRoute
{
public:
    FlightGeoRoute(double radius = 0.0, double maxSpeed = 0.0);

    void addGeoPoint(const QGeoCoordinate &geoPoint);
    void updateRoute();

    QVariantList geoPoints() const { return mGeoPoints; }
    QVariantList geoPath() const { return mGeoPath; }
    void setGeoPoints(const QVariantList &list) { mGeoPoints = list; }
    double maxSpeed() const { return mMaxSpeed; }

private:
    void createLastGeoSegment(const QGeoCoordinate &point);
    QPointF toPlaneCoordinate(const QGeoCoordinate &origin, const QGeoCoordinate &point);
    QGeoCoordinate toGeoCoordinate(const QGeoCoordinate &origin, const QPointF &point);

private:
    QVariantList mGeoPoints;
    QVariantList mGeoPath;
    QGeoCoordinate mStart;
    QGeoCoordinate mMiddle;
    double mRadius;
    double mMaxSpeed;
};

#endif // FLIGHTGEOROUTE_H
