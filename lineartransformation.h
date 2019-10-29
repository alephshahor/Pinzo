#ifndef LINEARTRANSFORMATION_H
#define LINEARTRANSFORMATION_H

#include "include/image.h"

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <cmath>

namespace Ui {
class LinearTransformation;
}

class LinearTransformation : public QWidget
{
    Q_OBJECT

private:

    Ui::LinearTransformation *ui;
    struct Line{
      double slope;
      int yoffset;
      int xoffset;
      double calculateValue(int x){
          return ceil((x - xoffset)*slope + yoffset);
      }
    };

    QVector<QPoint> mPoints;
    QVector<double> mTransformedSpaceKeys;
    QVector<double> mTransformedSpaceValues;
    Image& mImage;

public:
    explicit LinearTransformation(Image& image, QWidget *parent = nullptr);
    ~LinearTransformation();


signals:
    void imageChanged(Image image);

private:
    void addPoint();
    void swap(int i, int j);
    void transformImage();
    void applyTransformation(Line line, int minRange, int maxRange);
    double calculateSlope(QPoint p1, QPoint p2);
    int exists(int x);
    void sortPoints();
    void quicksort(int lo, int hi);
    int partition(int lo, int hi);
    void calculateTransformedSpace();
    void calculateSpaceKeys();
    void constructPlot();

};

#endif // LINEARTRANSFORMATION_H
