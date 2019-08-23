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
    qreal randomize(qreal value, qreal maxError);

private:
    qreal mCurrentBeamAngle = 0.0;
    QVariantList mPoints;
    QHash<int, int> mLostDatagramsCounters;
    constexpr static int MAX_DATAGRAM_LOST_COUNT = 3;
    constexpr static qreal DISTANCE_MAX_ERROR = 200.0;
    constexpr static qreal AZIMUTH_MAX_ERROR = 0.2;
};

#endif // FLIGHTPOSITIONS_H
