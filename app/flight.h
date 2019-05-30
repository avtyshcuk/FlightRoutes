#ifndef FLIGHT_H
#define FLIGHT_H

#include <QObject>
#include <QList>

#include "flightsegment.h"

class Flight : public QObject
{
    Q_OBJECT
public:
    explicit Flight(QObject *parent = nullptr);

    int size() const { return mFlightSegments.size(); }
    QVariantList points() const { return mPoints; }
    FlightSegment flightSegment(int index) const;
    void addPoint(const QPointF &point, bool hasVirtual = true);
    bool isLastSegmentValid() const { return mIsLastSegmentValid; }
    void setPixelRadius(double radius) { mPixelRadius = radius; }
    bool hasVirtualPart() const { return mHasVirtualPart; }
    void addVirtualPoint(const QPointF &point);
    bool isLastVirtual() const;
    void updateFlight(int index, const QPointF &point);
    bool isUpdateValid() const { return mIsUpdateValid; }
    double maxSpeed() const { return mMaxSpeed; }
    double radius() const;

signals:
    void pointsChanged();
    void startFlightUpdate(int size);
    void endFlightUpdate();
    void hasVirtualPartChanged();
    void flightPartUpdate(int index);

private:
    FlightSegment createSegment(int index, const QPointF &point,
                                bool isVirtual, bool *isValid = nullptr);
    FlightSegment appendSegment();
    void updateSegment(const FlightSegment &segment);

private:
    QVariantList mPoints;
    QList<FlightSegment> mFlightSegments;
    bool mIsLastSegmentValid = true;
    double mPixelRadius;
    bool mHasVirtualPart = false;
    bool mIsUpdateValid = false;
    double mMaxSpeed;
    double mRadius;
};

#endif // FLIGHT_H
