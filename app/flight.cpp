#include "flight.h"

Flight::Flight(QObject *parent)
    : QObject(parent)
    , mMaxSpeed(255.5) // 920 km/h
{

}

void Flight::addPoint(const QPointF &point, bool hasVirtual)
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

    // Real point has been added
    const bool hasFirstSegment = mPoints.size() == 1;
    if (hasFirstSegment || !hasVirtual) {
        mPoints << point;
        emit pointsChanged();
        updateSegment(appendSegment());

        // Virtual segment is possible now
        if (hasFirstSegment) {
            mHasVirtualPart = hasVirtual;
            emit hasVirtualPartChanged();
        }
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

    auto segment = appendSegment();
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

void Flight::updateFlight(int index, const QPointF &point)
{
    mIsUpdateValid = true;

    for (int i = index; i < mFlightSegments.size() || i == size(); ++i) {
        // First, last and previous segment hanlding
        if (i < 2 || i == index) {

            // Previous segment update
            if (i == index) {
                const int segmentIndex = i > 0 ? i - 1 : 0;
                auto segment = createSegment(i, point, true, &mIsUpdateValid);
                mFlightSegments.replace(segmentIndex, segment);
                emit flightPartUpdate(segmentIndex);
            }
            continue;
        }

        const auto &previousSegment = mFlightSegments.at(i - 2);
        QPointF start = i == 2 ? previousSegment.line().p1()
                               : previousSegment.manoeuvre().exitPoint();
        QPointF middle = i == index + 1 ? point
                                        : mFlightSegments.at(i - 1).line().p1();
        QPointF end = mFlightSegments.at(i - 1).line().p2();

        auto manoeuvre = Manoeuvre(start, middle, end, mPixelRadius);
        if (!manoeuvre.isValid()) {
            mIsUpdateValid = false;
        }
        auto currentSegment = FlightSegment(QLineF(middle, end), true, manoeuvre);
        mFlightSegments.replace(i - 1, currentSegment);
        emit flightPartUpdate(i - 1);
    }
}

FlightSegment Flight::createSegment(int index, const QPointF &point,
                                    bool isVirtual, bool *isValid)
{
    if (index == 0) {
        const QPointF &end = mPoints.at(index + 1).toPointF();
        return FlightSegment(QLineF(point, end), isVirtual);
    }

    if (index == 1) {
        const QPointF &start = mPoints.at(index - 1).toPointF();
        return FlightSegment(QLineF(start, point), isVirtual);
    }

    const auto &previousSegment = mFlightSegments.at(index - 2);
    auto isSecondSegment = index == 2;
    QPointF start = isSecondSegment ? previousSegment.line().p1()
                                    : previousSegment.manoeuvre().exitPoint();
    QPointF middle = previousSegment.line().p2();
    QPointF end = point;

    auto manoeuvre = Manoeuvre(start, middle, end, mPixelRadius);
    if (isValid && !manoeuvre.isValid()) {
        *isValid = false;
    }
    return FlightSegment(QLineF(middle, end), isVirtual, manoeuvre);
}

FlightSegment Flight::appendSegment()
{
    const int index = mPoints.size() - 1;
    return createSegment(index, mPoints.at(index).toPointF(), false);
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

double Flight::radius() const
{
    // Assume that Boeing-747 has maximum speed 920km/h
    // G-force is 3g, so radius = v*v/g-force -> 21770m
    return mMaxSpeed * mMaxSpeed / 3;
}
