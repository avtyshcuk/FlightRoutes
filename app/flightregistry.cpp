#include "flightregistry.h"

#include <QDebug>

FlightRegistry::FlightRegistry(QObject *parent)
    : QObject(parent)
{

}

void FlightRegistry::resetActiveFlight()
{
    mActiveFlight = std::make_unique<Flight>();
    mFlightModel = std::make_unique<FlightModel>();
    mFlightModel->setFlight(mActiveFlight.get());
    emit flightModelChanged();
    mActiveFlightId = -1;

    const double radius = mActiveFlight->radius();
    const double maxSpeed = mActiveFlight->maxSpeed();
    mActiveGeoRoute = FlightGeoRoute(radius, maxSpeed);

    mHasActiveFlight = true;
    emit hasActiveFlightChanged();
}

void FlightRegistry::finalizeActiveFlight()
{
    if (mActiveFlight->size() > 0) {
        mActiveFlightId = freeFlightId();
        mFlightGeoRoutes[mActiveFlightId] = mActiveGeoRoute;
    } else {
        mActiveFlightId = -1;
    }
    emit activeFlightIdChanged();

    mHasActiveFlight = false;
    emit hasActiveFlightChanged();
}

void FlightRegistry::addNewPoint(const QGeoCoordinate &geoPoint, const QPointF &point,
                                 bool hasVirtual)
{
    if (!mHasActiveFlight) {
        return;
    }

    mActiveFlight->addPoint(point, hasVirtual);
    if (mActiveFlight->isLastSegmentValid()) {
        mActiveGeoRoute.addGeoPoint(geoPoint);
    }
}

void FlightRegistry::addVirtualPoint(const QPointF &point)
{
    mActiveFlight->addVirtualPoint(point);
}

QVariantList FlightRegistry::activeGeoPoints() const
{
    return mActiveGeoRoute.geoPoints();
}

QVariantList FlightRegistry::activeGeoPath() const
{
    return mActiveGeoRoute.geoPath();
}

void FlightRegistry::setCurrentPixelRadius(double radius)
{
    mActiveFlight->setPixelRadius(radius);
}

QVariantList FlightRegistry::flightGeoPoints(int flightId) const
{
    return mFlightGeoRoutes[flightId].geoPoints();
}

void FlightRegistry::prepareFlightUpdate(int flightId, const QVariantList &points,
                                         double radius)
{
    resetActiveFlight();
    setCurrentPixelRadius(radius);

    for (int i = 0; i < points.size(); ++i) {
        auto point = points.at(i).toPointF();
        auto geoPoint = mFlightGeoRoutes[flightId].geoPoints().at(i);
        addNewPoint(geoPoint.value<QGeoCoordinate>(), point, false);
    }

    mActiveFlightId = flightId;
    emit activeFlightIdChanged();

    mActiveGeoRoute = mFlightGeoRoutes[flightId];

    setModifiedIndex(-1);
    mIsBeingModified = true;
    emit isBeingModifiedChanged();
}

void FlightRegistry::setModifiedIndex(int modifiedIndex)
{
    mModifiedIndex = modifiedIndex;
    emit modifiedIndexChanged();
}

void FlightRegistry::finalizeFlightUpdate(int index, const QGeoCoordinate &geoPoint)
{
    if (mActiveFlight->isUpdateValid()) {
        QVariantList geoPoints = mActiveGeoRoute.geoPoints();
        geoPoints.replace(index, QVariant::fromValue(geoPoint));
        mActiveGeoRoute.setGeoPoints(geoPoints);
        mActiveGeoRoute.updateRoute();

        mFlightGeoRoutes[mActiveFlightId] = mActiveGeoRoute;
    }

    mHasActiveFlight = false;
    emit hasActiveFlightChanged();
    
    setModifiedIndex(-1);
    mIsBeingModified = false;
    emit isBeingModifiedChanged();
}

void FlightRegistry::updateActiveFlight(int index, const QPointF &point)
{
    mActiveFlight->updateFlight(index, point);
}

int FlightRegistry::freeFlightId()
{
    // Keys in map are sorted, first absent index is free ID
    const auto &keys = mFlightGeoRoutes.keys();
    for (int index = 0; index < keys.size(); ++index) {
        if (keys[index] != index) {
            return index;
        }
    }

    // If list does not contain free ones, take the biggest
    return keys.size();
}
