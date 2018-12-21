#include "flightsegment.h"

FlightSegment::FlightSegment(const QLineF &line,
                             const Manoeuvre &manoeuvre)
    : mLine(line)
    , mManoeuvre(manoeuvre)
{

}
