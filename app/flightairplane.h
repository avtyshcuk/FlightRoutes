#ifndef FLIGHTAIRPLANE_H
#define FLIGHTAIRPLANE_H

#include <QQuickPaintedItem>
#include <QPainter>

class FlightAirPlane : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit FlightAirPlane(QQuickPaintedItem *parent = nullptr);

    void paint(QPainter *painter) override;
};

#endif // FLIGHTAIRPLANE_H
