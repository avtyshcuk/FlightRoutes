#ifndef FLIGHTPOSITIONS_H
#define FLIGHTPOSITIONS_H

#include <QUdpSocket>
#include <QGeoCoordinate>
#include <QHash>

class FlightPositions : public QUdpSocket
{
    Q_OBJECT
public:
    explicit FlightPositions(QObject *parent = nullptr);

    void setFlightPlayerPoints(const QVariantList &points);

public slots:
    void sendFlightPoints(const QGeoCoordinate &samCoordinate,
                          qreal range, qreal angle);
private:
    bool isDatagramNeeded(int pointIndex);

private:
    qreal mCurrentBeamAngle = 0.0;
    QVariantList mPoints;
    QHash<int, int> mLostDatagramsCounters;
    const int MAX_DATAGRAM_LOST_COUNT = 3;
};

#endif // FLIGHTPOSITIONS_H
