#ifndef IMAGEADJUSTER_H
#define IMAGEADJUSTER_H

#include <QWidget>
#include "include/image.h"

namespace Ui {
class ImageAdjuster;
}

class ImageAdjuster : public QWidget
{
    Q_OBJECT

public:
    ImageAdjuster(Image& image, QWidget* parent);
    ~ImageAdjuster();

    Image getImage();
    void setImage(const Image &image);


    int limitNumber(int num);

    void adjustImage(int brightnessAmount, double contrastAmount);

    float contrastCorrectionFactor(float contrastAmount);

    void processSliderInput();
    void processTextInput();

    /* Haven't been implemented*/
    double calculateBrightness();
    double calculateContrast();

private:
    void reportChange(Image image);
    void refreshLabels();

signals:
    void imageChanged(Image image);

private:

    Image mImage;
    Ui::ImageAdjuster *ui;
};

#endif // IMAGEADJUSTER_H
