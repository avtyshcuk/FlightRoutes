#include "flightplayer.h"

#include "flightregistry.h"

FlightPlayer::FlightPlayer(FlightRegistry *registry, QObject *parent)
    : QObject(parent)
    , mRegistry(registry)
{
    QObject::connect(&mPlayerTimer, &QTimer::timeout, this, [this]{

        removeFinishedPoints();
        if (mFlightPoints.isEmpty()) {
            setIsStopped(true);
        }

        updateFlightPoints();
        emit flightPointsUpdated();
    });
}

void FlightPlayer::setIsPlaying(bool isPlaying)
{
    if (isPlaying) {
        if (mIsStopped) {
            calculateSegments();
            mIsStopped = false;
            emit isStoppedChanged();
        }

        mPlayerTimer.start(mInterval);
    } else {
        mPlayerTimer.stop();
    }

    mIsPlaying = isPlaying;
    emit isPlayingChanged();
}

void FlightPlayer::setIsStopped(bool isStopped)
{
    setIsPlaying(false);

    mFlightPoints.clear();
    mSegments.clear();
    emit flightPointsUpdated();

    mIsStopped = isStopped;
    emit isStoppedChanged();
}

QVariantList FlightPlayer::points() const
{
    QVariantList points;
    auto it = mFlightPoints.constBegin();
    while (it != mFlightPoints.constEnd()) {
        points << QVariant::fromValue(it.value().position);
        ++it;
    }

    return points;
}

QVariantList FlightPlayer::azimuths() const
{
    QVariantList azimuths;
    auto it = mFlightPoints.constBegin();
    while (it != mFlightPoints.constEnd()) {
        azimuths << QVariant::fromValue(it.value().azimuth);
        ++it;
    }

    return azimuths;
}

void FlightPlayer::calculateSegments()
{
    mSegments.clear();
    mFlightPoints.clear();

    const auto routes = mRegistry->flightGeoRoutes();
    auto it = routes.constBegin();
    while (it != routes.constEnd()) {
        const auto points = it.value().geoPath();
        const int id = it.key();

        mSegments[id].reserve(points.size() - 1);
        mFlightPoints[id] = FlightPoint{ points.first().value<QGeoCoordinate>() };

        // Currently speed is constant for all flight
        const double speed = it.value().maxSpeed();
        for (int i = 1; i < points.size(); ++i) {
            const auto &firstPoint = points.at(i - 1).value<QGeoCoordinate>();
            const auto &secondPoint = points.at(i).value<QGeoCoordinate>();
            const double distance = firstPoint.distanceTo(secondPoint);
            const double azimuth = firstPoint.azimuthTo(secondPoint);

            mSegments[id] << Segment{ firstPoint, distance, azimuth, speed };
        }

        ++it;
    }
}

void FlightPlayer::updateFlightPoints()
{
    auto it = mFlightPoints.begin();
    while (it != mFlightPoints.end()) {
        const int id = it.key();
        auto &point = it.value();

        const int index = point.index;
        auto currentSegment = mSegments[id].at(index);
        const double delta = currentSegment.speed * mInterval / 1000;
        const double requiredDistance = point.distance + delta;

        if (requiredDistance > currentSegment.distance) {
            double deltaDistance = requiredDistance - currentSegment.distance;

            // no more distance at this segment, let's 'borrow' from next one
            while (deltaDistance > 0) {
                if (index + 1 < mSegments[id].size()) {
                    currentSegment = mSegments[id].at(++point.index);
                    deltaDistance -= currentSegment.distance;
                } else {
                    deltaDistance = 0;
                    point.isFinished = true;
                    break;
                }
            }

            point.distance = currentSegment.distance + deltaDistance;
        } else {
            point.distance += delta;
        }

        const double &azimuth = currentSegment.azimuth;
        const QGeoCoordinate &start = currentSegment.start;
        point.position = start.atDistanceAndAzimuth(point.distance, azimuth);
        point.azimuth = currentSegment.azimuth;

        ++it;
    }
}

void FlightPlayer::removeFinishedPoints()
{
    auto it = mFlightPoints.begin();
    while (it != mFlightPoints.end()) {
        if (it.value().isFinished) {
            it = mFlightPoints.erase(it);
        } else {
            ++it;
        }
    }
}
