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

AbsoluteHistogram::AbsoluteHistogram(Image image, QWidget *parent) :
    QWidget(parent),
    Histogram(image),
    ui(new Ui::AbsoluteHistogram),
    mCurrentType(Lightness)
{
    ui->setupUi(this);
    mImageRange = pow(2,mImage.getImageDepth());
    connect(ui -> changeButton, &QPushButton::clicked,
            this, &AbsoluteHistogram::changeDisplayType);
}

AbsoluteHistogram::~AbsoluteHistogram()
{
    delete ui;
}

int AbsoluteHistogram::calculateMin()
{

    for(int i = 0; i < mVPixelValue.size(); i++){
        if(mVPixelValue[i] > 0){
           return i;
        }
    }

    return mVPixelValue.size() - 1;
}

int AbsoluteHistogram::calculateMax()
{

    for(int i = mImageRange - 1; i >= 0; i--){
        if(mVPixelValue[i] > 0){
           return i;
        }
    }

    return 0;
}

double AbsoluteHistogram::calculateMean()
{
    double mean = 0;
    for(int i = 0; i < mVPixelValue.size(); i++){
        mean += (i * mVPixelValue[i]);
    }
    mean /= numberOfPixels();
    return mean;
}

double AbsoluteHistogram::calculateStdDeviation()
{
    int mean = calculateMean();
    int sum = 0;

    for(int i = 0; i < mVPixelValue.size(); i++){
        sum += (mVPixelValue[i] - mean) * (mVPixelValue[i] - mean);
    }
    return sqrt(sum / numberOfPixels());
}

void AbsoluteHistogram::createHistogram()
{
      ui -> customPlot -> addGraph();
      QColor color(20+200/4.0,70*(1.6/4.0), 150, 150);
      ui -> customPlot -> graph( )-> setLineStyle(QCPGraph::lsLine);
      ui -> customPlot -> graph() -> setPen(QPen(color.lighter(200)));
      ui -> customPlot -> graph() -> setBrush(QBrush(color));
      ui -> customPlot -> graph(0)-> setData(mVPixelKey,mVPixelValue);
      ui -> customPlot -> xAxis -> setRange(0, mImageRange);
      ui -> customPlot -> yAxis -> setRange(0, calculateModeFrequency());
      ui -> customPlot -> replot();
}

void AbsoluteHistogram::displayInfo()
{
    ui -> countLabel -> setText("Count: " + QString::number(numberOfPixels()));
    ui -> minLabel -> setText("Min: " + QString::number(calculateMin()));
    ui -> maxLabel -> setText("Max: " + QString::number(calculateMax()));
    ui -> meanLabel -> setText("Mean: " + QString::number(calculateMean()));
    ui -> modeLabel -> setText("Mode: " + QString::number(calculateModeValue()) + " (" +
                                          QString::number(calculateModeFrequency()) + ")");
    ui -> stdDevLabel -> setText("StdDev: " + QString::number(calculateStdDeviation()));
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

void AbsoluteHistogram::displayHistogram(){

    switch(mCurrentType){
        case Lightness:
        calculateHistogramValues(calculateColorLightnessValue);
        calculateHistogramKeys();
        createHistogram();
        displayInfo();
        break;

        case Red:
        calculateHistogramValues(calculateRedColorValue);
        calculateHistogramKeys();
        createHistogram();
        displayInfo();
        break;

        case Green:
        calculateHistogramValues(calculateGreenColorValue);
        calculateHistogramKeys();
        createHistogram();
        displayInfo();
        break;

        case Blue:
        calculateHistogramValues(calculateBlueColorValue);
        calculateHistogramKeys();
        createHistogram();
        displayInfo();
        break;
    }


}

void AbsoluteHistogram::changeDisplayType()
{
    if(mCurrentType == Blue){
        mCurrentType = Lightness;
    }else mCurrentType += 1;
    displayHistogram();
}

int AbsoluteHistogram::calculateColorLightnessValue(QColor pixel)
{
    return pixel.lightness();
}

int AbsoluteHistogram::calculateRedColorValue(QColor pixel)
{
    return pixel.red();
}

int AbsoluteHistogram::calculateBlueColorValue(QColor pixel)
{
    return pixel.blue();
}

int AbsoluteHistogram::calculateGreenColorValue(QColor pixel)
{
    return pixel.green();
}

int AbsoluteHistogram::getCurrentType() const
{
    return mCurrentType;
}

void AbsoluteHistogram::setCurrentType(int value)
{
    mCurrentType = value;
}



