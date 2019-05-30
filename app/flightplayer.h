#ifndef FLIGHTPLAYER_H
#define FLIGHTPLAYER_H

#include <QTimer>
#include <QHash>
#include <QList>
#include <QGeoCoordinate>

class FlightRegistry;

class FlightPlayer : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isPlaying READ isPlaying WRITE setIsPlaying NOTIFY isPlayingChanged)
    Q_PROPERTY(bool isStopped READ isStopped WRITE setIsStopped NOTIFY isStoppedChanged)

public:
    explicit FlightPlayer(FlightRegistry *registry, QObject *parent = nullptr);

    bool isPlaying() const { return mIsPlaying; }
    void setIsPlaying(bool isPlaying);
    bool isStopped() const { return mIsStopped; }
    void setIsStopped(bool isStopped);

    Q_INVOKABLE QVariantList points() const;

signals:
    void isPlayingChanged();
    void isStoppedChanged();
    void flightPointsUpdated();

private:
    struct Segment
    {
        QGeoCoordinate start;
        double distance;
        double azimuth;
        double speed;
    };

    void calculateSegments();

    struct FlightPoint
    {
        QGeoCoordinate position;
        int index = 0;
        double distance = 0.0;
        bool isFinished = false;
    };

    void updateFlightPoints();
    void removeFinishedPoints();

private:
    FlightRegistry *mRegistry;
    QTimer mPlayerTimer;
    const int mInterval = 1000;
    bool mIsPlaying = false;
    bool mIsStopped = true;
    QHash<int, QList<Segment>> mSegments;
    QHash<int, FlightPoint> mFlightPoints;
};

#endif // FLIGHTPLAYER_H
