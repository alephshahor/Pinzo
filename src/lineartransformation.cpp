#include "../include/lineartransformation.h"

QVector<int> LinearTransformation::transformedSpace() const
{
    return mTransformedSpace;
}

void LinearTransformation::setTransformedSpace(const QVector<int> &transformedSpace)
{
    mTransformedSpace = transformedSpace;
}

LinearTransformation::LinearTransformation(int imageDepth):
    mSpaceRange(pow(2,imageDepth))
{
    mTransformedSpace = QVector<int>(mSpaceRange);
}

void LinearTransformation::addPoint(QPoint point)
{
    mPoints.append(point);
}

void LinearTransformation::calculateTransformedSpace()
{
    if(mPoints.empty()){
        throw "No points declared\n";
    }

    for(int i = 0; i < mPoints.size() - 1; i++){
        double slope = calculateSlope(mPoints[i], mPoints[i+1]);
        Line line = {slope, mPoints[i].y(), mPoints[i].x()};
        applyTransformation(line, mPoints[i].x(), mPoints[i+1].x());
    }
}

void LinearTransformation::applyTransformation(LinearTransformation::Line line, int minRange, int maxRange)
{
     for(int i = minRange; i <= maxRange; i++){
         mTransformedSpace[i] = line.calculateValue(i);
     }
}

double LinearTransformation::calculateSlope(QPoint p1, QPoint p2)
{
    return static_cast<double>(p2.y() - p1.y()) / static_cast<double>(p2.x() - p1.x());
}


