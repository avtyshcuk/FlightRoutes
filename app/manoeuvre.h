#ifndef MANOEUVRE_H
#define MANOEUVRE_H

#include <QPointF>

class Manoeuvre
{
public:
    Manoeuvre() = default;
    Manoeuvre(const QPointF &start, const QPointF &middle,
              const QPointF &final, double radius);

    bool isNull() const { return mIsNull; }
    bool isValid() const { return mIsValid; }
    bool isLeftTurn() const { return mIsLeftTurn; }
    QPointF circlePoint() const { return mCircle; }
    double radius() const { return mRadius; }
    QPointF exitPoint() const { return mExit; }
    double startAngle() const { return mStartAngle; }
    double spanAngle() const { return mSpanAngle; }
    void toGeoManoeuvre();

private:
    void calculate();
    void calculateAngles(double startAngle, double endAngle);
    bool isEqualToZero(double value);

private:
    bool mIsNull = true;
    bool mIsValid = false;
    QPointF mStart;
    QPointF mMiddle;
    QPointF mFinal;
    double mRadius;
    bool mIsLeftTurn;
    QPointF mCircle;
    QPointF mExit;
    double mStartAngle;
    double mSpanAngle;
};

#endif // MANOEUVRE_H
