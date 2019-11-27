#include "imageadjuster.h"
#include "ui_imageadjuster.h"

#include <QSlider>
#include <QMessageBox>
#include <QDebug>

ImageAdjuster::ImageAdjuster(Image& image, QWidget *parent) :
    QWidget(parent),
    mImage(image),
    ui(new Ui::ImageAdjuster)
{
    ui->setupUi(this);
//    connect(ui -> brightnessSlider, &QSlider::valueChanged,
//            this, &ImageAdjuster::processSliderInput);
//    connect(ui -> contrastSlider, &QSlider::valueChanged,
//            this, &ImageAdjuster::processSliderInput);
//    connect(ui -> brightnessText, &QLineEdit::returnPressed,
//            this, &ImageAdjuster::processTextInput);
//    connect(ui -> contrastText, &QLineEdit::returnPressed,
//            this, &ImageAdjuster::processTextInput);

    connect(ui -> rMeanText, &QLineEdit::returnPressed,
            this, &ImageAdjuster::processMean);
    connect(ui -> gMeanText, &QLineEdit::returnPressed,
            this, &ImageAdjuster::processMean);
    connect(ui -> bMeanText, &QLineEdit::returnPressed,
            this, &ImageAdjuster::processMean);

    connect(ui -> rDeviationText, &QLineEdit::returnPressed,
            this, &ImageAdjuster::processDeviation);
    connect(ui -> gDeviationText, &QLineEdit::returnPressed,
            this, &ImageAdjuster::processDeviation);
    connect(ui -> bDeviationText, &QLineEdit::returnPressed,
            this, &ImageAdjuster::processDeviation);

//    connect(ui -> rgbCheckBox, &QCheckBox::clicked,
//            this, &ImageAdjuster::processRgbCheckbox);
//    connect(ui -> refreshButton, &QPushButton::pressed,
//            this, &ImageAdjuster::refreshTextAndSliders);

    mHistogram = new AbsoluteHistogram(mImage, nullptr);
    mHistogram -> calculateHistogramKeys();
    refreshMeanAndDeviation();
    ui -> rgbCheckBox -> setChecked(true);
}


ImageAdjuster::~ImageAdjuster()
{
    delete ui;
}


Image ImageAdjuster::getImage()
{
    return mImage;
}

void ImageAdjuster::setImage(const Image &image)
{
    mImage = image;
}

void ImageAdjuster::toGrayscale(Image& image)
{
    QRgb *st = (QRgb *) image.getImage().bits();
    quint64 pixelCount = image.getWidth() * image.getHeight();

    for (quint64 p = 0; p < pixelCount; p++) {

        float average = (qRed(st[p]) + qGreen(st[p]) + qBlue(st[p]))/3;
        st[p] = QColor(average,
                       average,
                       average).rgb();
    }
}

// Using LUMA
double ImageAdjuster::calculateBrightness()
{
    double summary = 0.0;
    for(int i = 0; i < mImage.getWidth() ; i++){
        for(int j = 0; j < mImage.getHeight(); j++){
            QColor color = mImage.getImage().pixel(i,j);
            double red = 0.299 * color.red();
            double blue = 0.114 * color.blue();
            double green = 0.587 * color.green();
            summary += (red + blue + green);
        }
    }
    summary /= (mImage.getWidth() * mImage.getHeight());
    return summary;
}

void ImageAdjuster::grayscaleMode()
{
    QString mean = ui -> rMeanText -> text();
    QString deviation = ui -> rDeviationText -> text();
    ui -> gMeanText -> setText(mean);
    ui -> gDeviationText -> setText(deviation);
    ui -> bMeanText -> setText(mean);
    ui -> bDeviationText -> setText(deviation);
}

void ImageAdjuster::refreshMeanAndDeviation()
{
    mHistogram -> calculateHistogramValues(Histogram::calculateRedColorValue);
    ui -> rMeanText -> setText(QString::number(mHistogram->calculateMean()));
    ui -> rDeviationText -> setText(QString::number(mHistogram->calculateStdDeviation()));

    mHistogram -> calculateHistogramValues(Histogram::calculateGreenColorValue);
    ui -> gMeanText -> setText(QString::number(mHistogram->calculateMean()));
    ui -> gDeviationText -> setText(QString::number(mHistogram->calculateStdDeviation()));

    mHistogram -> calculateHistogramValues(Histogram::calculateBlueColorValue);
    ui -> bMeanText -> setText(QString::number(mHistogram->calculateMean()));
    ui -> bDeviationText -> setText(QString::number(mHistogram->calculateStdDeviation()));

    processMean();
    processDeviation();

}

void ImageAdjuster::adjustImage(int brightnessAmount, double contrastAmount)
{

    Image adjustedImage(mImage);
    QRgb *st = (QRgb *) adjustedImage.getImage().bits();
    quint64 pixelCount = adjustedImage.getWidth() * adjustedImage.getHeight();

    float contrastFactor = contrastCorrectionFactor(contrastAmount);

    for (quint64 p = 0; p < pixelCount; p++) {

        st[p] = QColor(limitNumber( (qRed(st[p]) - 128) * contrastAmount + 128 + brightnessAmount),
                       limitNumber( (qGreen(st[p]) - 128) * contrastAmount + 128 + brightnessAmount),
                       limitNumber( (qBlue(st[p]) - 128) * contrastAmount + 128 + brightnessAmount)).rgb();
    }
    reportChange(adjustedImage);
}

void ImageAdjuster::adjustImage()
{

    Image adjustedImage(mImage);
    QRgb *st = (QRgb *) adjustedImage.getImage().bits();
    quint64 pixelCount = adjustedImage.getWidth() * adjustedImage.getHeight();

    for (quint64 p = 0; p < pixelCount; p++) {
        st[p] = QColor(limitNumber( (qRed(st[p]) * mRedAlpha)  + mRedBias),
                       limitNumber( (qGreen(st[p]) * mGreenAlpha)   + mGreenBias),
                       limitNumber( (qBlue(st[p]) * mBlueAlpha)  + mBlueBias)).rgb();
    }

    reportChange(adjustedImage);
}

float ImageAdjuster::contrastCorrectionFactor(float contrastAmount)
{
    float correctionFactor = 259*(contrastAmount + 255);
    correctionFactor /= 255*(259 - contrastAmount);
    return correctionFactor;
}

void ImageAdjuster::processSliderInput()
{
    int brightnessAmount = ui -> brightnessSlider -> value();
    double contrastAmount = (static_cast<double>(ui -> contrastSlider -> value()) / 10);

    ui -> brightnessText -> setText(QString::number(ui -> brightnessSlider -> value()));
    ui -> contrastText -> setText(QString::number(static_cast<double>(ui -> contrastSlider -> value()) / 10));

    adjustImage(brightnessAmount, contrastAmount);
}

void ImageAdjuster::processTextInput()
{
    int brightnessAmount = ui -> brightnessText -> text().toInt();
    double contrastAmount = static_cast<double>(ui -> contrastText -> text().toDouble());

    int brightnessMinAmount = ui -> brightnessSlider -> minimum();
    int brightnessMaxAmount = ui -> brightnessSlider -> maximum();

    double contrastMinAmount = static_cast<double>(ui -> contrastSlider -> minimum()) / 10;
    double contrastMaxAmount = static_cast<double>(ui -> contrastSlider -> maximum()) / 10;


    if(brightnessAmount < brightnessMinAmount
    || brightnessAmount > brightnessMaxAmount){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Brightness out of range [" + QString::number(brightnessMinAmount)
                                                            + "," + QString::number(brightnessMaxAmount) + "]");
        messageBox.setFixedSize(500,200);
    }else if(contrastAmount < contrastMinAmount
    || contrastAmount > contrastMaxAmount){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Contrast out of range [" + QString::number(contrastMinAmount)
                                                            + "," + QString::number(contrastMaxAmount) + "]");
        messageBox.setFixedSize(500,200);
    }else{
        ui -> brightnessSlider -> setValue(ui -> brightnessText -> text().toInt());
        ui -> contrastSlider -> setValue(ui -> contrastText -> text().toDouble() * 10);
        adjustImage(brightnessAmount, contrastAmount);
    }
}


void ImageAdjuster::refreshTextAndSliders(){
    refreshMeanAndDeviation();
    ui -> brightnessSlider -> setValue(0);
    ui -> brightnessText -> setText("0");
    ui -> contrastSlider -> setValue(10 );
    ui -> contrastText -> setText("1");
}

void ImageAdjuster::processDeviation()
{
    mRedAlpha = calculateAlpha(Red);

    mGreenAlpha = calculateAlpha(Green);

    mBlueAlpha = calculateAlpha(Blue);

    mRedBias = calculateBias(Red);

    mGreenBias = calculateBias(Green);

    mBlueBias = calculateBias(Blue);

    adjustImage();
}

void ImageAdjuster::processMean()
{
    mRedBias = calculateBias(Red);

    mGreenBias = calculateBias(Green);

    mBlueBias = calculateBias(Blue);

    adjustImage();
}

void ImageAdjuster::processRgbCheckbox(){
    if(ui -> rgbCheckBox -> checkState() == false){
        ui -> gMeanText -> setDisabled(true);
        ui -> gDeviationText -> setDisabled(true);
        ui -> bMeanText -> setDisabled(true);
        ui -> bDeviationText -> setDisabled(true);
        ui -> rChannelLabel -> setText("(GRAY)");
        ui -> gChannelLabel -> setText("(X)");
        ui -> bChannelLabel -> setText("(X)");
        grayscaleMode();
    }else{
        ui -> gMeanText -> setDisabled(false);
        ui -> gDeviationText -> setDisabled(false);
        ui -> bMeanText -> setDisabled(false);
        ui -> bDeviationText -> setDisabled(false);
        ui -> rChannelLabel -> setText("(R)");
        ui -> gChannelLabel -> setText("(G)");
        ui -> bChannelLabel -> setText("(B)");
    }
}

int ImageAdjuster::limitNumber(int num)
{
    if(num >= 255)
        return 255;
    else if(num <= 0)
        return 0;
    else return num;
}

void ImageAdjuster::reportChange(Image image)
{
    emit imageChanged(image);
}

double ImageAdjuster::calculateAlpha(mColor color)
{

    double desiredDeviation;

    switch(color){
        case Red:
        mHistogram -> calculateHistogramValues(Histogram::calculateRedColorValue);
        desiredDeviation = ui -> rDeviationText -> text().toDouble();
        break;

        case Green:
        mHistogram -> calculateHistogramValues(Histogram::calculateGreenColorValue);
        desiredDeviation = ui -> gDeviationText -> text().toDouble();
        break;

        case Blue:
        mHistogram -> calculateHistogramValues(Histogram::calculateBlueColorValue);
        desiredDeviation = ui -> bDeviationText -> text().toDouble();
        break;

    }

    double currentDeviation = mHistogram -> calculateStdDeviation();

    return desiredDeviation/currentDeviation;
}

double ImageAdjuster::calculateBias(ImageAdjuster::mColor color)
{

    double alpha = calculateAlpha(color);
    double desiredMean;

    switch(color){
        case Red:
        mHistogram -> calculateHistogramValues(Histogram::calculateRedColorValue);
        desiredMean = ui -> rMeanText -> text().toDouble();
        break;

        case Green:
        mHistogram -> calculateHistogramValues(Histogram::calculateGreenColorValue);
        desiredMean = ui -> gMeanText -> text().toDouble();
        break;

        case Blue:
        mHistogram -> calculateHistogramValues(Histogram::calculateBlueColorValue);
        desiredMean = ui -> bMeanText -> text().toDouble();
        break;

    }

    double currentMean = mHistogram -> calculateMean();

    return desiredMean - (currentMean * alpha);
}
