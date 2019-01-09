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
    Q_PROPERTY(int activeFlightId READ activeFlightId NOTIFY activeFlightIdChanged)
    Q_PROPERTY(bool isBeingModified READ isBeingModified NOTIFY isBeingModifiedChanged)
    Q_PROPERTY(int modifiedIndex READ modifiedIndex NOTIFY modifiedIndexChanged)
    Q_PROPERTY(FlightModel *flightModel READ flightModel NOTIFY flightModelChanged)

public:
    explicit FlightRegistry(QObject *parent = nullptr);

    using FlightPtr = std::unique_ptr<Flight>;
    using FlightModelPtr = std::unique_ptr<FlightModel>;

    Q_INVOKABLE void resetActiveFlight();
    Q_INVOKABLE void finalizeActiveFlight();
    Q_INVOKABLE void addNewPoint(const QGeoCoordinate &geoPoint, const QPointF &point,
                                 bool hasVirtual = true);
    Q_INVOKABLE void addVirtualPoint(const QPointF &point);
    Q_INVOKABLE QVariantList activeGeoPoints() const;
    Q_INVOKABLE QVariantList activeGeoPath() const;
    Q_INVOKABLE void setCurrentPixelRadius(double radius);
    Q_INVOKABLE QVariantList flightGeoPoints(int flightId) const;
    Q_INVOKABLE void prepareFlightUpdate(int flightId, const QVariantList &points,
                                         double radius);
    Q_INVOKABLE void setModifiedIndex(int modifiedIndex);
    Q_INVOKABLE void finalizeFlightUpdate(int index, const QGeoCoordinate &geoPoint);
    Q_INVOKABLE void updateActiveFlight(int index, const QPointF &point);

    bool hasActiveFlight() const { return mHasActiveFlight; }
    int activeFlightId() const { return mActiveFlightId; }
    bool isBeingModified() const { return mIsBeingModified; }
    int modifiedIndex() const { return mModifiedIndex; }
    FlightModel *flightModel() { return mFlightModel.get(); }

signals:
    void hasActiveFlightChanged();
    void activeFlightIdChanged();
    void flightModelChanged();
    void isBeingModifiedChanged();
    void modifiedIndexChanged();

private:
    int freeFlightId();

private:
    bool mHasActiveFlight = false;
    int mActiveFlightId = -1;
    bool mIsBeingModified = false;
    int mModifiedIndex = -1;
    FlightPtr mActiveFlight;
    FlightModelPtr mFlightModel;
    FlightGeoRoute mActiveGeoRoute;
    QMap<int, FlightGeoRoute> mFlightGeoRoutes;
};

#endif // FLIGHTREGISTRY_H
