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
    void adjustImage(double rBrightnessAmount, double rContrastAmount,
                     double gBrightnessAmount, double gContrastAmount,
                     double bBrightnessAmount, double bContrastAmount);

    float contrastCorrectionFactor(float contrastAmount);

    void processSliderInput();
    void processTextInput();
    void processRgbCheckbox();
    void processMeansAndDeviations();

    /* Haven't been implemented*/
    double calculateBrightness();
    double calculateContrast();

    void equaliseMeansAndDeviation();

private:

    void calculateInitialHistogramValues();
    void reportChange(Image image);
    void refreshLabels();
    double calculateAlpha(mColor color);
    double calculateBias(mColor color);

signals:
    void imageChanged(Image image);


};

#endif // IMAGEADJUSTER_H
