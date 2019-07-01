#include "flighttrackerserver.h"

#include "flightplayer.h"

FlightTrackerServer::FlightTrackerServer(QObject *parent)
    : QObject(parent)
{
    connect(&mBeamPosition, &BeamPosition::beamParametersUpdated, &mFlightPositions,
            &FlightPositions::sendFlightPoints);
}

void FlightTrackerServer::setFlightPlayer(FlightPlayer *flightPlayer)
{
    mFlightPlayer = flightPlayer;
    connect(mFlightPlayer, &FlightPlayer::flightPointsUpdated, [this]{
        mFlightPositions.setFlightPlayerPoints(mFlightPlayer->points());
    });
}
