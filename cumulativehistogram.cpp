#include "cumulativehistogram.h"
#include "ui_cumulativehistogram.h"
#include "ui_histogram.h"

#include <QVector>
#include <cmath>

CumulativeHistogram::CumulativeHistogram(Image image, QWidget* parent) :
    Histogram(image, parent)
{
}


void CumulativeHistogram::calculateHistogramValues(int(*func)(QColor))
{
    QVector<double> vPixelValue(mImageRange);

    for(int i = 0; i < mImage.getImage().width(); i++){
        for(int j = 0; j < mImage.getImage().height(); j++){
            QColor pixel = mImage.getImage().pixel(i,j);
            int pixelValue = func(pixel);
            vPixelValue[pixelValue] += 1.0;
        }
    }

    QVector<double> vPixelValueCumulative(mImageRange);
    vPixelValueCumulative[0] = vPixelValue[0];
    for(int i = 1; i < vPixelValue.size(); i++){
        vPixelValueCumulative[i] = vPixelValueCumulative[i - 1] + vPixelValue[i];
    }

      setVPixelValue(vPixelValueCumulative);
}

double CumulativeHistogram::calculateEntropy(){

    double summatory = 0;
    int imageDimension = mImage.getImage().width() * mImage.getImage().height();
    for(int i = 0; i < mVPixelValue.size(); i++){
        double iProbability = mVPixelValue[i] / imageDimension;
        summatory += iProbability * log(iProbability);
    }

    return (-1)*summatory;
}

void CumulativeHistogram::displayInfo()
{
    ui -> countLabel -> setText("Count: " + QString::number(numberOfPixels()));
    ui -> minLabel -> setText("Min: " + QString::number(calculateMin()));
    ui -> maxLabel -> setText("Max: " + QString::number(calculateMax()));
    ui -> meanLabel -> setText("Mean: " + QString::number(calculateMean()));
    ui -> modeLabel -> setText("Mode: " + QString::number(calculateModeValue()) + " (" +
                                          QString::number(calculateModeFrequency()) + ")");
    ui -> stdDevLabel -> setText("StdDev: " + QString::number(calculateStdDeviation()));
}


CumulativeHistogram::~CumulativeHistogram()
{
    delete ui;
}


