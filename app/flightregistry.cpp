#include "flightregistry.h"

FlightRegistry::FlightRegistry(QObject *parent)
    : QObject(parent)
{

}

void FlightRegistry::createNewFlight()
{
    mActiveFlight = std::make_unique<Flight>();
    mFlightModel = std::make_unique<FlightModel>();
    mFlightModel->setFlight(mActiveFlight.get());
    emit flightModelChanged();

    mActiveGeoRoute = FlightGeoRoute();

    mHasActiveFlight = true;
    emit hasActiveFlightChanged();
}

void FlightRegistry::finalizeActiveFlight()
{
    auto flightId = freeFlightId();
    mFlightGeoRoutes[flightId] = mActiveGeoRoute;

    mHasActiveFlight = false;
    emit hasActiveFlightChanged();
}

void FlightRegistry::updateActiveFlight(const QGeoCoordinate &geoPoint,
                                        const QPointF &point)
{
    if (!mHasActiveFlight) {
        return;
    }

    mActiveFlight->addPoint(point);
    if (mActiveFlight->isLastSegmentValid()) {
        mActiveGeoRoute.addGeoPoint(geoPoint);
    }
}

void FlightRegistry::updateVirtualPart(const QPointF &point)
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

int FlightRegistry::freeFlightId()
{
    // Keys in map are sorted, first absent index is free ID
    const auto& keys = mFlightGeoRoutes.keys();
    for (int index = 0; index < keys.size(); ++index) {
        if (keys[index] != index) {
            return index;
        }
    }

    // If list does not contain free ones, take the biggest
    return keys.size();
}
