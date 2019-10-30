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





