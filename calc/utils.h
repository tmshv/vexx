#ifndef UTILS_H
#define UTILS_H

#include <math.h>

inline float findDist(QPointF val1, QPointF val2)
{
    QPointF dist = val2 - val1;
    return sqrt( pow(dist.x(), 2) + pow(dist.y(), 2) );
}

inline float findLength(QPointF val)
{
    return findDist(QPointF(0,0), val);
}

#endif // UTILS_H
