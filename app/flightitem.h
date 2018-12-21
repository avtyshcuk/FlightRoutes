#ifndef FLIGHTITEM_H
#define FLIGHTITEM_H

#include <QQuickPaintedItem>
#include <QPainter>

class FlightItem : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QPointF startPoint READ startPoint WRITE setStartPoint NOTIFY startPointChanged)
    Q_PROPERTY(QPointF endPoint READ endPoint WRITE setEndPoint NOTIFY endPointChanged)
    Q_PROPERTY(QRectF manoeuvreRect READ manoeuvreRect WRITE setManoeuvreRect NOTIFY manoeuvreRectChanged)
    Q_PROPERTY(int manoeuvreStartAngle READ manoeuvreStartAngle WRITE setManoeuvreStartAngle
               NOTIFY manoeuvreStartAngleChanged)
    Q_PROPERTY(int manoeuvreSpanAngle READ manoeuvreSpanAngle WRITE setManoeuvreSpanAngle
               NOTIFY manoeuvreSpanAngleChanged)
    Q_PROPERTY(bool isVirtualLink READ isVirtualLink WRITE setIsVirtualLink NOTIFY isVirtualLinkChanged)

public:
    explicit FlightItem(QQuickPaintedItem *parent = nullptr);

    void paint(QPainter *painter) override;

    QPointF startPoint() const { return mStartPoint; }
    QPointF endPoint() const { return mEndPoint; }
    QRectF manoeuvreRect() const { return mManoeuvreRect; }
    int manoeuvreStartAngle() const { return mManoeuvreStartAngle; }
    int manoeuvreSpanAngle() const { return mManoeuvreSpanAngle; }
    bool isVirtualLink() const { return mIsVirtualLink; }

    void setStartPoint(const QPointF &startPoint);
    void setEndPoint(const QPointF &endPoint);
    void setManoeuvreRect(const QRectF &manoeuvreRect);
    void setManoeuvreStartAngle(int manoeuvreStartAngle);
    void setManoeuvreSpanAngle(int manoeuvreSpanAngle);
    void setIsVirtualLink(bool isVirtual);

signals:
    void startPointChanged();
    void endPointChanged();
    void manoeuvreRectChanged();
    void manoeuvreStartAngleChanged();
    void manoeuvreSpanAngleChanged();
    void isVirtualLinkChanged();

private:
    QPointF mStartPoint;
    QPointF mEndPoint;
    QRectF mManoeuvreRect;
    int mManoeuvreStartAngle;
    int mManoeuvreSpanAngle;
    bool mIsVirtualLink;
};

#endif // FLIGHTITEM_H
