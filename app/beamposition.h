#ifndef FLIGHTPOSITIONSNOTIFIER_H
#define FLIGHTPOSITIONSNOTIFIER_H

#include <QUdpSocket>
#include <QGeoCoordinate>

class BeamPosition : public QUdpSocket
{
    Q_OBJECT
public:
    explicit BeamPosition(QObject *parent = nullptr);

signals:
    void beamParametersUpdated(const QGeoCoordinate &coordinate,
                               qreal range, qreal angle);
};

#endif // FLIGHTPOSITIONSNOTIFIER_H
