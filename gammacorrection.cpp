#include "gammacorrection.h"
#include "ui_gammacorrection.h"

#include <QMessageBox>
#include <QDebug>
#include <cmath>

GammaCorrection::GammaCorrection(Image image, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GammaCorrection),
    mImage(image),
    mGammaValue(1),
    mMinGammaValue(0),
    mMaxGammaValue(5)
{
    ui->setupUi(this);
    connect(ui -> valueSlider, &QSlider::valueChanged,
            this, &GammaCorrection::proccessSliderInput);
    connect(ui -> valueText, &QLineEdit::returnPressed,
            this, &GammaCorrection::proccessTextInput);
}

GammaCorrection::~GammaCorrection()
{
    delete ui;
}

void GammaCorrection::proccessSliderInput(){
    double gammaValue = static_cast<double>(ui -> valueSlider -> value()) / 10;
    mGammaValue = gammaValue;
    applyGamma();

    ui -> valueText -> setText(QString::number(gammaValue));
}

void GammaCorrection::proccessTextInput(){
    double gammaValue = ui -> valueText -> text().toDouble();
    if(gammaValue < mMinGammaValue
     ||gammaValue > mMaxGammaValue){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Gamma value out of range [" + QString::number(mMinGammaValue)
                                                            + "," + QString::number(mMaxGammaValue) + "]");
        messageBox.setFixedSize(500,200);
    }else{
        mGammaValue = gammaValue;
        applyGamma();
        ui -> valueSlider -> setValue(gammaValue * 10);
    }
}

void GammaCorrection::applyGamma()
{
    Image adjustedImage(mImage);
    int intensityValues = pow(2,mImage.getImageDepth());

    QRgb *st = (QRgb *) adjustedImage.getImage().bits();
    quint64 pixelCount = adjustedImage.getWidth() * adjustedImage.getHeight();

    for (quint64 p = 0; p < pixelCount; p++) {

        double red_in   =  static_cast<double>(qRed(st[p])) / (intensityValues  - 1);
        double green_in =  static_cast<double>(qGreen(st[p])) / (intensityValues  - 1);
        double blue_in  =  static_cast<double>(qBlue(st[p])) / (intensityValues  - 1);

        double red_out = pow(red_in, mGammaValue) * (intensityValues  - 1);
        double green_out = pow(green_in, mGammaValue) * (intensityValues  - 1);
        double blue_out = pow(blue_in, mGammaValue) * (intensityValues  - 1);

        st[p] = QColor(red_out, green_out, blue_out).rgb();
    }

    emit imageChanged(adjustedImage);

}
