#include "histogram.h"
#include "ui_histogram.h"

#include <QBarSet>
#include <QPoint>
#include <vector>
#include <QChartView>
#include <iostream>
#include <cmath>

Histogram::Histogram(Image image, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Histogram),
    mImage(image),
    mCurrentType(Lightness)
{
    ui->setupUi(this);
    mImageRange = pow(2,mImage.getImageDepth());
    connect(ui -> changeButton, &QPushButton::clicked,
            this, &Histogram::changeDisplayType);
    displayHistogram();
}

Histogram::Histogram(Histogram &histogram)
{
    mImage = histogram.getImage();
    mCurrentType = histogram.getCurrentType();
    mVPixelKey = histogram.getVPixelKey();
    mVPixelValue = histogram.getVPixelValue();

}

Histogram::~Histogram()
{
    delete ui;
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

int Histogram::calculateMin()
{

    for(int i = 0; i < mVPixelValue.size(); i++){
        if(mVPixelValue[i] > 0){
           return i;
        }
    }

    return mVPixelValue.size() - 1;
}

int Histogram::calculateMax()
{

    for(int i = 255; i >= 0; i--){
        if(mVPixelValue[i] > 0){
           return i;
        }
    }

    return 0;
}

double Histogram::calculateMean()
{
    double mean = 0;
    for(int i = 0; i < mVPixelValue.size(); i++){
        mean += (i * mVPixelValue[i]);
    }
    mean /= numberOfPixels();
    return mean;
}

double Histogram::calculateStdDeviation()
{
    int mean = calculateMean();
    int sum = 0;

    for(int i = 0; i < mVPixelValue.size(); i++){
        sum += (mVPixelValue[i] - mean) * (mVPixelValue[i] - mean);
    }
    return sqrt(sum / numberOfPixels());
}

int Histogram::numberOfPixels()
{
    return mImage.getImage().width() * mImage.getImage().height();
}

void Histogram::displayInfo()
{
    ui -> countLabel -> setText("Count: " + QString::number(numberOfPixels()));
    ui -> minLabel -> setText("Min: " + QString::number(calculateMin()));
    ui -> maxLabel -> setText("Max: " + QString::number(calculateMax()));
    ui -> meanLabel -> setText("Mean: " + QString::number(calculateMean()));
    ui -> modeLabel -> setText("Mode: " + QString::number(calculateModeValue()) + " (" +
                                          QString::number(calculateModeFrequency()) + ")");
    ui -> stdDevLabel -> setText("StdDev: " + QString::number(calculateStdDeviation()));
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
    QVector<double> vPixelKey(256);
    for(int i = 0; i < 256; i++){
        vPixelKey[i] = static_cast<double>(i);
    }

    setVPixelKey(vPixelKey);
}

void Histogram::calculateHistogramValues(int(*func)(QColor))
{
    QVector<double> vPixelValue(256);

    for(int i = 0; i < mImage.getImage().width(); i++){
        for(int j = 0; j < mImage.getImage().height(); j++){
            QColor pixel = mImage.getImage().pixel(i,j);
            int pixelValue = func(pixel);
            vPixelValue[pixelValue] += 1.0;
        }
    }


      setVPixelValue(vPixelValue);
}

void Histogram::createHistogram()
{
      ui -> customPlot -> addGraph();
      QColor color(20+200/4.0,70*(1.6/4.0), 150, 150);
      ui -> customPlot -> graph( )-> setLineStyle(QCPGraph::lsLine);
      ui -> customPlot -> graph() -> setPen(QPen(color.lighter(200)));
      ui -> customPlot -> graph() -> setBrush(QBrush(color));
      ui -> customPlot -> graph(0)-> setData(mVPixelKey,mVPixelValue);
      ui -> customPlot -> xAxis -> setRange(0, 300);
      ui -> customPlot -> yAxis -> setRange(0, calculateModeFrequency());
      ui -> customPlot -> replot();
}

void Histogram::displayHistogram(){

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

void Histogram::changeDisplayType()
{
    if(mCurrentType == Blue){
        mCurrentType = Lightness;
    }else mCurrentType += 1;
    displayHistogram();
}

int Histogram::calculateColorLightnessValue(QColor pixel)
{
    return pixel.lightness();
}

int Histogram::calculateRedColorValue(QColor pixel)
{
    return pixel.red();
}

int Histogram::calculateBlueColorValue(QColor pixel)
{
    return pixel.blue();
}

int Histogram::calculateGreenColorValue(QColor pixel)
{
    return pixel.green();
}

QVector<double> Histogram::getVPixelKey() const
{
    return mVPixelKey;
}

QVector<double> Histogram::getVPixelValue() const
{
    return mVPixelValue;
}

int Histogram::getCurrentType() const
{
    return mCurrentType;
}

void Histogram::setCurrentType(int value)
{
    mCurrentType = value;
}

Image Histogram::getImage() const
{
    return mImage;
}

void Histogram::setImage(const Image &image)
{
    mImage = image;
}


