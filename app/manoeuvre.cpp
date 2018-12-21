#include <limits>
#include <QtMath>

#include "manoeuvre.h"

Manoeuvre::Manoeuvre(const QPointF &start, const QPointF &middle,
                     const QPointF &final, double radius)
    : mStart(start)
    , mMiddle(middle)
    , mFinal(final)
    , mRadius(radius)
{
    mIsNull = false;
    calculate();
}

void Manoeuvre::calculate()
{
    // General equation of line between first and middle points
    auto A = mStart.y() - mMiddle.y();
    auto B = mMiddle.x() - mStart.x();

    // Check cross product sign whether final point lies on left side
    auto crossProduct = (B*(mFinal.y() - mStart.y()) + A*(mFinal.x() - mStart.x()));

    // All three points lie on the same line
    if (isEqualToZero(crossProduct)) {
        mIsValid = true;
        mCircle = mExit = mMiddle;
        return;
    }

    mIsLeftTurn = crossProduct > 0;
    auto lineNorm = A*A + B*B;
    auto exitSign = mIsLeftTurn ? 1 : -1;
    auto projection = exitSign*mRadius * qSqrt(lineNorm);

    // Center lies on perpendicular to middle point
    if (!isEqualToZero(A) && !isEqualToZero(B)) {
        auto C = -B*mStart.y() - A*mStart.x();
        auto right = (projection - C)/A - (mMiddle.x()*lineNorm + A*C) / (B*B);
        mCircle.ry() = right / (A/B + B/A);
        mCircle.rx() = (projection - B*mCircle.y() - C) / A;
    } else {
        // Entering line is perpendicular to either x- or y-axis
        auto deltaY = isEqualToZero(A) ? 0 : exitSign*mRadius;
        auto deltaX = isEqualToZero(B) ? 0 : exitSign*mRadius;
        mCircle.ry() = mMiddle.y() + deltaY;
        mCircle.rx() = mMiddle.x() + deltaX;
    }

    // Check if final point is outside manouevre circle
    auto circleDiffX = mFinal.x() - mCircle.x();
    auto circleDiffY = mFinal.y() - mCircle.y();
    auto distance = qSqrt(circleDiffX*circleDiffX + circleDiffY*circleDiffY);

    mIsValid = distance > mRadius;

    // Does not make sence to calculate futher
    if (!mIsValid)
        return;

    // Length of hypotenuse from final point to exit point
    auto beta = qAtan2(mCircle.y() - mFinal.y(), mCircle.x() - mFinal.x());
    auto alpha = qAsin(mRadius / distance);
    auto length = qSqrt(distance*distance - mRadius*mRadius);

    // Depends on position of final point find exit point
    mExit.rx() = mFinal.x() + length*qCos(beta + exitSign*alpha);
    mExit.ry() = mFinal.y() + length*qSin(beta + exitSign*alpha);

    // Finally calculate start/span angles
    auto startAngle = qAtan2(mCircle.y() - mMiddle.y(), mMiddle.x() - mCircle.x());
    auto endAngle = qAtan2(mCircle.y() - mExit.y(), mExit.x() - mCircle.x());
    calculateAngles(startAngle, endAngle);
}

void Manoeuvre::toGeoManoeuvre()
{
    // Geo polar coordinates have '0' pointed north with clockwise direction
    qreal startAngle = qAtan2(mMiddle.x() - mCircle.x(), mMiddle.y() - mCircle.y());
    qreal endAngle = qAtan2(mExit.x() - mCircle.x(), mExit.y() - mCircle.y());

    calculateAngles(startAngle, endAngle);
}

void Manoeuvre::calculateAngles(double startAngle, double endAngle)
{
    mStartAngle = startAngle < 0 ? startAngle + 2*M_PI : startAngle;
    endAngle = endAngle < 0 ? endAngle + 2*M_PI : endAngle;

    auto smallSpan = qFabs(endAngle - mStartAngle);
    auto bigSpan = 2*M_PI - qFabs(mStartAngle - endAngle);
    bool isZeroCrossed = mStartAngle > endAngle;

    if (!mIsLeftTurn) {
        mSpanAngle = isZeroCrossed ? bigSpan : smallSpan;
    } else {
        mSpanAngle = isZeroCrossed ? smallSpan : bigSpan;
    }
}

bool Manoeuvre::isEqualToZero(double value)
{
    return qFabs(value) <= std::numeric_limits<double>::min();
}
