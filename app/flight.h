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
    void addPoint(const QPointF &point);
    bool isLastSegmentValid() const { return mIsLastSegmentValid; }
    void setPixelRadius(double radius) { mPixelRadius = radius; }
    bool hasVirtualPart() const { return mHasVirtualPart; }
    void addVirtualPoint(const QPointF &point);
    bool isLastVirtual() const;

signals:
    void pointsChanged();
    void startFlightUpdate(int size);
    void endFlightUpdate();
    void hasVirtualPartChanged();
    void flightPartUpdate(int index);

private:
    FlightSegment createLastSegment();
    void updateSegment(const FlightSegment &segment);

private:
    QVariantList mPoints;
    QList<FlightSegment> mFlightSegments;
    bool mIsLastSegmentValid = true;
    double mPixelRadius;
    bool mHasVirtualPart = false;
};

#endif // FLIGHT_H
