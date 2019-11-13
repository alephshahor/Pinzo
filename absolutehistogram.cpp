#include "absolutehistogram.h"
#include "ui_absolutehistogram.h"

#include "histogram.h"
#include "ui_histogram.h"

#include <QBarSet>
#include <QPoint>
#include <vector>
#include <QChartView>
#include <iostream>
#include <cmath>

AbsoluteHistogram::AbsoluteHistogram(Image& image, QWidget* parent) :
    Histogram(image, parent)
{}

AbsoluteHistogram::~AbsoluteHistogram()
{
    delete ui;
}

void AbsoluteHistogram::displayInfo()
{
    Histogram::displayInfo();
    ui -> entropyLabel -> setText("Entropy: " + QString::number(calculateEntropy()));
}

void AbsoluteHistogram::calculateHistogramValues(int(*func)(QColor))
{
    QVector<double> vPixelValue(mImageRange);

    for(int i = 0; i < mImage.getImage().width(); i++){
        for(int j = 0; j < mImage.getImage().height(); j++){
            QColor pixel = mImage.getImage().pixel(i,j);
            int pixelValue = func(pixel);
            vPixelValue[pixelValue] += 1.0;
        }
    }


      setVPixelValue(vPixelValue);
}

double AbsoluteHistogram::calculateEntropy(){

    double summatory = 0;
    int imageDimension = mImage.getImage().width() * mImage.getImage().height();
    for(int i = 0; i < mVPixelValue.size(); i++){
        double iProbability = mVPixelValue[i] / imageDimension;
        // If we allow probability to be zero then the logarith is -infinity and
        // we are storing a NaN value in the summatory variable, which causes
        // all successive sums to be NaN.
        if(iProbability != 0.0)
        summatory += iProbability * log2(iProbability);
    }


    return (-1)*summatory;
}





