#include "flight.h"

Flight::Flight(QObject *parent)
    : QObject(parent)
{

}

void Flight::addPoint(const QPointF &point)
{
    // First point
    if (mPoints.isEmpty()) {
        mPoints << point;
        emit pointsChanged();
        return;
    }

    // Does not make sense to add same points
    if (!isLastVirtual() && point == mPoints.last()) {
        mIsLastSegmentValid = false;
        return;
    }

    // First segment
    if (mPoints.size() == 1) {
        mPoints << point;
        emit pointsChanged();
        updateSegment(createLastSegment());

        // Virtual segment is possible now
        mHasVirtualPart = true;
        emit hasVirtualPartChanged();
        return;
    }

    // Segment is ready to became real one
    if (mFlightSegments.last().isValid()) {
        mFlightSegments.last().setIsVirtual(false);
        emit flightPartUpdate(size() - 1);
        emit pointsChanged();
    }
}

void Flight::addVirtualPoint(const QPointF &point)
{
    if (!isLastVirtual()) {
        mPoints << point;
    } else {
        mPoints.last() = point;
    }

    auto segment = createLastSegment();
    segment.setIsVirtual(true);
    updateSegment(segment);
}

bool Flight::isLastVirtual() const
{
    if (mFlightSegments.isEmpty()) {
        return false;
    }
    return mFlightSegments.last().isVirtual();
}

FlightSegment Flight::createLastSegment()
{
    const int index = mPoints.size() - 1;
    if (index == 1) {
        QPointF start = mPoints.at(index - 1).toPointF();
        QPointF end = mPoints.at(index).toPointF();
        return FlightSegment(QLineF(start, end));
    }

    auto previousSegment = mFlightSegments.at(index - 2);
    auto isSecondSegment = index == 2;
    QPointF start = isSecondSegment ? previousSegment.line().p1()
                                    : previousSegment.manoeuvre().exitPoint();
    QPointF middle = previousSegment.line().p2();
    QPointF end = mPoints.at(index).toPointF();

    auto manoeuvre = Manoeuvre(start, middle, end, mPixelRadius);
    return FlightSegment(QLineF(middle, end), manoeuvre);
}

void Flight::updateSegment(const FlightSegment &segment)
{
    mIsLastSegmentValid = segment.isValid();

    if (!isLastVirtual()) {
        emit startFlightUpdate(size());
        mFlightSegments << segment;
        emit endFlightUpdate();
    } else {
        mFlightSegments.last() = segment;
        emit flightPartUpdate(size() - 1);
    }
}

FlightSegment Flight::flightSegment(int index) const
{
    return mFlightSegments.at(index);
}
