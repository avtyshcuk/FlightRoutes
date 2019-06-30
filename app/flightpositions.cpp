#include "flightpositions.h"

#include "flightplayer.h"
#include "beamposition.h"

FlightPositions::FlightPositions(QObject *parent)
    : QUdpSocket(parent)
{

}

void FlightPositions::setFlightPlayer(FlightPlayer *flightPlayer)
{
    mFlightPlayer = flightPlayer;
}

void FlightPositions::setBeamPosition(BeamPosition *beamPosition)
{
    mBeamPosition = beamPosition;

    connect(mBeamPosition, &BeamPosition::beamPositionUpdated, [this]{
        auto samPosition = mBeamPosition->samPosition();
        auto samBeamRange = mBeamPosition->samBeamRange();
        auto samBeamAngle = mBeamPosition->samBeamAngle();
        for (auto point : mFlightPlayer->points()) {
            auto coordinate = point.value<QGeoCoordinate>();
            auto distance = samPosition.distanceTo(coordinate);
            auto azimuth = samPosition.azimuthTo(coordinate);

            if (distance < samBeamRange && azimuth > mCurrentBeamAngle && azimuth < samBeamAngle) {
                QString data = QString("%1,%2").arg(coordinate.latitude()).arg(coordinate.longitude());
                writeDatagram(data.toUtf8(), QHostAddress::LocalHost, 45455);
            }
        }

        mCurrentBeamAngle = samBeamAngle;
    });
}
