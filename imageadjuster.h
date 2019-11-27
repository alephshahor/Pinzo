#ifndef IMAGEADJUSTER_H
#define IMAGEADJUSTER_H

#include <QWidget>
#include "include/image.h"
#include "histogram.h"
#include "absolutehistogram.h"

namespace Ui {
class ImageAdjuster;
}

class ImageAdjuster : public QWidget
{
    Q_OBJECT

private:
    enum mColor {Red, Green, Blue};
    double mRedBias;
    double mGreenBias;
    double mBlueBias;
    double mRedAlpha;
    double mGreenAlpha;
    double mBlueAlpha;
    Image mImage;
    Histogram* mHistogram;
    Ui::ImageAdjuster *ui;

public:
    ImageAdjuster(Image& image, QWidget* parent);
    ~ImageAdjuster();

    Image getImage();
    void setImage(const Image &image);

    static void toGrayscale(Image &image);

    int limitNumber(int num);

    void adjustImage(int brightnessAmount, double contrastAmount);
    void adjustImage();
//    void adjustImageAlpha();
//    void adjustImageBias();


    float contrastCorrectionFactor(float contrastAmount);

    void processSliderInput();
    void processTextInput();
    void processRgbCheckbox();
    void processMeansAndDeviations();

    void processMean();
    void processDeviation();

    /* Haven't been implemented*/
    double calculateBrightness();
    double calculateContrast();

    void grayscaleMode();

private:

    void refreshMeanAndDeviation();
    void reportChange(Image image);
    void refreshLabels();
    double calculateAlpha(mColor color);
    double calculateBias(mColor color);
    void refreshTextAndSliders();

signals:
    void imageChanged(Image image);


};

#endif // IMAGEADJUSTER_H
