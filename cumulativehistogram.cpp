#include "cumulativehistogram.h"
#include "ui_cumulativehistogram.h"
#include "ui_histogram.h"

#include <QVector>
#include <cmath>

CumulativeHistogram::CumulativeHistogram(Image& image, QWidget* parent) :
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


void CumulativeHistogram::displayInfo()
{
    Histogram::displayInfo();
    ui -> entropyLabel -> setText("");
}

CumulativeHistogram::~CumulativeHistogram()
{
    delete ui;
}


