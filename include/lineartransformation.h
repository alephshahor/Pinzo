#ifndef LINEARTRANSFORMATION_H
#define LINEARTRANSFORMATION_H

#include <QVector>
#include <QPoint>
#include <cmath>

class LinearTransformation {

private:

    struct Line{
      double slope;
      int yoffset;
      int xoffset;
      int calculateValue(int x ){
          return ceil((x - xoffset)*slope + yoffset);
      }
    };

    QVector<QPoint> mPoints;
    QVector<int> mTransformedSpace;
    int mSpaceRange;


public:

    LinearTransformation(int imageDepth);
    void addPoint(QPoint point);

    void calculateTransformedSpace();

    QVector<int> transformedSpace() const;
    void setTransformedSpace(const QVector<int> &transformedSpace);

private:
    void applyTransformation(Line line, int minRange, int maxRange);
    double calculateSlope(QPoint p1, QPoint p2);


};
#endif
