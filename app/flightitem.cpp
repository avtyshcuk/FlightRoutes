#include "flightitem.h"

FlightItem::FlightItem(QQuickPaintedItem *parent)
    : QQuickPaintedItem (parent)
{

}

void FlightItem::paint(QPainter *painter)
{
    // We cannot draw without at least two points
    if (mStartPoint.isNull() || mEndPoint.isNull()) {
        return;
    }

    QPen pen;
    pen.setStyle(mIsVirtualLink ? Qt::DashLine : Qt::SolidLine);
    pen.setColor(Qt::black);

    painter->setPen(pen);
    painter->setRenderHints(QPainter::Antialiasing, true);

    // First two points do not have manoeuvre
    if (mManoeuvreRect.isNull()) {
        painter->drawLine(mStartPoint, mEndPoint);

        // If manouvre is invalid show "disable" hint for user
        if (mIsVirtualLink) {
            QPen pen = painter->pen();
            pen.setColor(Qt::red);
            painter->setPen(pen);
            painter->drawText(mEndPoint - QPointF(5, -5), QString("x"));
            pen.setColor(Qt::black);
        }

        return;
    }

    // Line + manoeuvre arc create trajectory link
    QPainterPath path(mStartPoint);
    path.arcTo(mManoeuvreRect, mManoeuvreStartAngle, mManoeuvreSpanAngle);
    path.lineTo(mEndPoint);
    painter->drawPath(path);
}

void FlightItem::setStartPoint(const QPointF &startPoint)
{
    if (startPoint != mStartPoint) {
        mStartPoint = startPoint;
        emit startPointChanged();
        update();
    }
}

void FlightItem::setEndPoint(const QPointF &endPoint)
{
    if (endPoint != mEndPoint) {
        mEndPoint = endPoint;
        emit endPointChanged();
        update();
    }
}

void FlightItem::setManoeuvreRect(const QRectF &manoeuvreRect)
{
    if (manoeuvreRect != mManoeuvreRect) {
        mManoeuvreRect = manoeuvreRect;
        emit manoeuvreRectChanged();
        update();
    }
}

void FlightItem::setManoeuvreStartAngle(int manoeuvreStartAngle)
{
    if (manoeuvreStartAngle != mManoeuvreStartAngle) {
        mManoeuvreStartAngle = manoeuvreStartAngle;
        emit manoeuvreStartAngleChanged();
        update();
    }
}

void FlightItem::setManoeuvreSpanAngle(int manoeuvreSpanAngle)
{
    if (manoeuvreSpanAngle != mManoeuvreSpanAngle) {
        mManoeuvreSpanAngle = manoeuvreSpanAngle;
        emit manoeuvreSpanAngleChanged();
        update();
    }
}

void FlightItem::setIsVirtualLink(bool isVirtual)
{
    if (isVirtual != mIsVirtualLink) {
        mIsVirtualLink = isVirtual;
        emit isVirtualLinkChanged();
        update();
    }
}
