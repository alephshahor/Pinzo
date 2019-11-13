#ifndef GAMMACORRECTION_H
#define GAMMACORRECTION_H

#include "include/image.h"
#include <QWidget>


namespace Ui {
class GammaCorrection;
}

class GammaCorrection : public QWidget
{
    Q_OBJECT

public:
    explicit GammaCorrection(Image image, QWidget *parent = nullptr);
    ~GammaCorrection();


signals:
    void imageChanged(Image image);

private:
    void proccessSliderInput();
    void proccessTextInput();
    void applyGamma();

private:
    Ui::GammaCorrection *ui;

    double mGammaValue;
    double mMinGammaValue;
    double mMaxGammaValue;

    Image mImage;

};

#endif // GAMMACORRECTION_H
