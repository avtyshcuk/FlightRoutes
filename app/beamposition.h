#ifndef FLIGHTPOSITIONSNOTIFIER_H
#define FLIGHTPOSITIONSNOTIFIER_H

#include <QUdpSocket>
#include <QGeoCoordinate>

class BeamPosition : public QUdpSocket
{
    Q_OBJECT
public:
    explicit BeamPosition(QObject *parent = nullptr);

    QGeoCoordinate samPosition() const { return mSamPosition; }
    qreal samBeamRange() const { return mSamBeamRange; }
    qreal samBeamAngle() const { return mSamBeamAngle; }

signals:
    void beamPositionUpdated();

private:
    QGeoCoordinate mSamPosition;
    qreal mSamBeamRange = 0.0;
    qreal mSamBeamAngle = 0.0;
};

#endif // FLIGHTPOSITIONSNOTIFIER_H
