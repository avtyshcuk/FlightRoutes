#ifndef FLIGHTREGISTRY_H
#define FLIGHTREGISTRY_H

#include <memory>

#include <QObject>
#include <QGeoCoordinate>

#include "flight.h"
#include "flightmodel.h"
#include "flightgeoroute.h"

class FlightRegistry : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool hasActiveFlight READ hasActiveFlight NOTIFY hasActiveFlightChanged)
    Q_PROPERTY(FlightModel *flightModel READ flightModel NOTIFY flightModelChanged)

public:
    explicit FlightRegistry(QObject *parent = nullptr);

    using FlightPtr = std::unique_ptr<Flight>;
    using FlightModelPtr = std::unique_ptr<FlightModel>;

    Q_INVOKABLE void createNewFlight();
    Q_INVOKABLE void finalizeActiveFlight();
    Q_INVOKABLE void updateActiveFlight(const QGeoCoordinate &geoPoint,
                                        const QPointF &point);
    Q_INVOKABLE void updateVirtualPart(const QPointF &point);
    Q_INVOKABLE QVariantList activeGeoPoints() const;
    Q_INVOKABLE QVariantList activeGeoPath() const;
    Q_INVOKABLE void setCurrentPixelRadius(double radius);

    bool hasActiveFlight() const { return mHasActiveFlight; }
    FlightModel *flightModel() { return mFlightModel.get(); }

signals:
    void hasActiveFlightChanged();
    void flightModelChanged();

private:
    int freeFlightId();

private:
    bool mHasActiveFlight = false;
    FlightPtr mActiveFlight;
    FlightModelPtr mFlightModel;
    FlightGeoRoute mActiveGeoRoute;
    QMap<int, FlightGeoRoute> mFlightGeoRoutes;
};

#endif // FLIGHTREGISTRY_H
