#include "flightpositions.h"

#include <QRandomGenerator>
#include <QDebug>

FlightPositions::FlightPositions(QObject *parent)
    : QUdpSocket(parent)
{

}

void FlightPositions::setFlightPlayerPoints(const QVariantList &points)
{
    mPoints = points;
}

void FlightPositions::sendFlightPoints(const QGeoCoordinate &samCoordinate,
                                       qreal range, qreal angle)
{
    for (int i = 0; i < mPoints.size(); ++i) {
        auto coordinate = mPoints.at(i).value<QGeoCoordinate>();
        auto distance = samCoordinate.distanceTo(coordinate);
        auto azimuth = samCoordinate.azimuthTo(coordinate);

        if (distance < range && azimuth > mCurrentBeamAngle && azimuth < angle) {
            if (!isDatagramNeeded(i)) {
                continue;
            }

            QString data = QString("%1,%2").arg(distance).arg(azimuth);
            writeDatagram(data.toUtf8(), QHostAddress::LocalHost, 45455);
        }
    }

    mCurrentBeamAngle = angle;
}

// Simulation data sending issues such as,
// noise, channel interruption, clouds, etc
bool FlightPositions::isDatagramNeeded(int pointIndex)
{
    if (mLostDatagramsCounters.contains(pointIndex)) {
        mLostDatagramsCounters[pointIndex]++;
        if (mLostDatagramsCounters[pointIndex] >= MAX_DATAGRAM_LOST_COUNT) {
            mLostDatagramsCounters.remove(pointIndex);
            return true;
        }
        return false;
    }

    auto randomValue = QRandomGenerator::global()->generateDouble();
    if (randomValue > 0.2) {
        return true;
    }

    if (randomValue < 0.05) {
        mLostDatagramsCounters[pointIndex] = 0;
    }

    return false;
}
