#ifndef FLIGHTTRACKERSERVER_H
#define FLIGHTTRACKERSERVER_H

#include <QObject>

#include "beamposition.h"
#include "flightpositions.h"

class FlightPlayer;

class FlightTrackerServer : public QObject
{
    Q_OBJECT
public:
    explicit FlightTrackerServer(QObject *parent = nullptr);

    void setFlightPlayer(FlightPlayer *flightPlayer);

private:
    BeamPosition mBeamPosition;
    FlightPositions mFlightPositions;
    FlightPlayer *mFlightPlayer;
};

#endif // FLIGHTTRACKERSERVER_H
