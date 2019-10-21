#include "histogram.h"
//include "ui_histogram.h"

#include <QBarSet>
#include <QPoint>
#include <vector>
#include <QChartView>
#include <iostream>
#include <cmath>

Histogram::Histogram(Image image) :
    mImage(image)
{
    mImageRange = pow(2,mImage.getImageDepth());
}

Histogram::Histogram(Histogram &histogram)
{
    mImage = histogram.getImage();
    mVPixelKey = histogram.getVPixelKey();
    mVPixelValue = histogram.getVPixelValue();
    mImageRange = histogram.getImageRange();
}

Histogram::~Histogram()
{

}



int Histogram::numberOfPixels()
{
    return mImage.getImage().width() * mImage.getImage().height();
}

void Histogram::setVPixelKey(const QVector<double> &vPixelKey)
{
    mVPixelKey = vPixelKey;
}

void Histogram::setVPixelValue(const QVector<double> &vPixelValue)
{
    mVPixelValue = vPixelValue;
}

void Histogram::calculateHistogramKeys()
{
    QVector<double> vPixelKey(mImageRange);
    for(int i = 0; i < mImageRange; i++){
        vPixelKey[i] = static_cast<double>(i);
    }

    setVPixelKey(vPixelKey);
}

int Histogram::calculateModeValue()
{
    int modeValue = 0;
    for(int i = 0; i < mVPixelValue.size(); i++){
        if(mVPixelValue[modeValue] < mVPixelValue[i]){
            modeValue = i;
        }
    }
    return modeValue;
}

int Histogram::calculateModeFrequency()
{
    int modeValue = calculateModeValue();
    return static_cast<int>(mVPixelValue[modeValue]);
}


int Histogram::getImageRange() const
{
    return mImageRange;
}

void Histogram::setImageRange(int imageRange)
{
    mImageRange = imageRange;
}

QVector<double> Histogram::getVPixelKey() const
{
    return mVPixelKey;
}

QVector<double> Histogram::getVPixelValue() const
{
    return mVPixelValue;
}

Image Histogram::getImage() const
{
    return mImage;
}

void Histogram::setImage(const Image &image)
{
    mImage = image;
}


