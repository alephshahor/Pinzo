#include "imageadjuster.h"
#include "ui_imageadjuster.h"

#include <QSlider>
#include <QDebug>

ImageAdjuster::ImageAdjuster(Image& image, QWidget *parent) :
    QWidget(parent),
    mPreviousBrigthnessAmount(0),
    mImage(image),
    ui(new Ui::ImageAdjuster)
{
    ui->setupUi(this);
    connect(ui -> brightnessSlider, &QSlider::valueChanged,
            this, &ImageAdjuster::processSliderInput);
    connect(ui -> contrastSlider, &QSlider::valueChanged,
            this, &ImageAdjuster::processSliderInput);
    connect(ui -> brightnessText, &QLineEdit::returnPressed,
            this, &ImageAdjuster::processTextInput);
    connect(ui -> brightnessText, &QLineEdit::returnPressed,
            this, &ImageAdjuster::processTextInput);
    refreshLabels();
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

    refreshLabels();
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
//    int brightnessAmount = ui -> brightnessSlider -> value() - getPreviousBrigthnessAmount();
//    setPreviousBrigthnessAmount(ui -> brightnessSlider -> value());
    int brightnessAmount = ui -> brightnessSlider -> value();
    double contrastAmount = (static_cast<double>(ui -> contrastSlider -> value()) / 10) + 0.05;

    ui -> brightnessText -> setText(QString::number(ui -> brightnessSlider -> value()));
    ui -> contrastText -> setText(QString::number(ui -> contrastSlider -> value() / 10));

    adjustImage(brightnessAmount, contrastAmount);
}

void ImageAdjuster::processTextInput()
{
    int brightnessAmount = ui -> brightnessText -> text().toInt() - getPreviousBrigthnessAmount();
    setPreviousBrigthnessAmount(ui -> brightnessText -> text().toInt());

    double contrastAmount = ui -> contrastText -> text().toInt() / 10;

    ui -> brightnessSlider -> setValue(ui -> brightnessText -> text().toInt());
    ui -> contrastSlider -> setValue(ui -> brightnessText -> text().toInt() * 10);

    adjustImage(brightnessAmount, contrastAmount);
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

void ImageAdjuster::refreshLabels()
{
    double brightness = calculateBrightness();
    ui -> brightnessLabel -> setText("Brightness: " + QString::number(brightness));

//    double contrast = calculateContrast();
//    ui -> brightnessLabel -> setText("Brightness: " + QString::number(brightness));
}

int ImageAdjuster::getPreviousContrastAmount() const
{
    return mPreviousContrastAmount;
}

void ImageAdjuster::setPreviousContrastAmount(int previousContrastAmount)
{
    mPreviousContrastAmount = previousContrastAmount;
}

int ImageAdjuster::getPreviousBrigthnessAmount() const
{
    return mPreviousBrigthnessAmount;
}

void ImageAdjuster::setPreviousBrigthnessAmount(int value)
{
    mPreviousBrigthnessAmount = value;
}


