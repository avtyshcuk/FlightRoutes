#ifndef FLIGHTPOSITIONS_H
#define FLIGHTPOSITIONS_H

#include <QUdpSocket>

class FlightPlayer;
class BeamPosition;

class FlightPositions : public QUdpSocket
{
    Q_OBJECT
public:
    explicit FlightPositions(QObject *parent = nullptr);

    void setFlightPlayer(FlightPlayer *flightPlayer);
    void setBeamPosition(BeamPosition *beamPosition);

private:
    qreal mCurrentBeamAngle = 0.0;
    FlightPlayer *mFlightPlayer;
    BeamPosition *mBeamPosition;
};

#endif // FLIGHTPOSITIONS_H
