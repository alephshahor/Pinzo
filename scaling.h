#ifndef SCALING_H
#define SCALING_H

#include <QWidget>
#include "include/image.h"

namespace Ui {
class Scaling;
}

class Scaling : public QWidget
{
    Q_OBJECT


private:

    void scaleGivenNewSize();
    void scaleGivenPercentage();

    void interpolate(Image& resizedImage, float xFactor, float yFactor);
    void nearestNeighbourInterpolation(Image& resizedImage, float xFactor, float yFactor);
    void bilinealInterpolation(Image& resizedImage, float xFactor, float yFactor);

    Image createResizedImage();


public:
    explicit Scaling(Image image, QWidget *parent = nullptr);
    ~Scaling();



signals:
    void imageChanged(Image image);

private:
    enum InterpolationMethods {NearestNeighbour, Bilineal};
    Image image;
    Ui::Scaling *ui;
};

#endif // SCALING_H
