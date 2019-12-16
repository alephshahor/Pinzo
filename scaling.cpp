#include "scaling.h"
#include "ui_scaling.h"

#include <math.h>
#include <QDebug>

Scaling::Scaling(Image image, QWidget *parent) :
    QWidget(parent),
    image(image),
    ui(new Ui::Scaling)
{
    ui->setupUi(this);
    connect(ui -> widthTextInput, &QLineEdit::returnPressed,
            this, &Scaling::scaleGivenNewSize);
    connect(ui -> heightTextInput, &QLineEdit::returnPressed,
            this, &Scaling::scaleGivenNewSize);
    connect(ui -> widthPercentage, &QLineEdit::returnPressed,
            this, &Scaling::scaleGivenPercentage);
    connect(ui -> heightPercentage, &QLineEdit::returnPressed,
            this, &Scaling::scaleGivenPercentage);

    ui -> widthTextInput -> setText(QString::number(this -> image.getWidth()));
    ui -> heightTextInput -> setText(QString::number(this -> image.getHeight()));

    ui -> widthPercentage -> setText("100");
    ui -> heightPercentage -> setText("100");
}

Scaling::~Scaling()
{
    delete ui;
}

void Scaling::scaleGivenNewSize()
{
    qDebug() << "Button pressed!\n";
    // Crear una nueva imagen con las nuevas dimensiones
    // Calcular el factor a multiplicar por los elementos en la TI

    int newWidth = ui -> widthTextInput -> text().toInt();
    int newHeight = ui -> heightTextInput -> text().toInt();

    float xFactor = float(newWidth) / float(this -> image.getWidth());
    float yFactor = float(newHeight) / float(this -> image.getHeight());

    ui -> widthPercentage -> setText(QString::number(xFactor * 100));
    ui -> heightPercentage -> setText(QString::number(yFactor * 100));

    Image newImage(this -> image, newWidth, newHeight);

    interpolate(newImage, xFactor, yFactor);

}

void Scaling::scaleGivenPercentage()
{

    float xFactor = float(ui -> widthPercentage -> text().toInt()) / 100;
    float yFactor = float(ui -> heightPercentage -> text().toInt()) / 100;

    int newWidth = ceil(this -> image.getWidth() * xFactor);
    int newHeight = ceil(this -> image.getHeight() * yFactor);

    ui -> widthTextInput -> setText(QString::number(newWidth));
    ui -> heightTextInput -> setText(QString::number(newHeight));

    Image newImage(this -> image, newWidth, newHeight);

    interpolate(newImage, xFactor, yFactor);
}


void Scaling::interpolate(Image& newImage, float xFactor, float yFactor)
{
    switch(this -> ui -> interpolationComboBox -> currentIndex()){
        case NearestNeighbour:
            nearestNeighbourInterpolation(newImage, xFactor, yFactor);
        break;

        case Bilineal:
            bilinealInterpolation(newImage, xFactor, yFactor);
        break;
    }
}

void Scaling::nearestNeighbourInterpolation(Image& resizedImage, float xFactor, float yFactor)
{
    // As we're going to apply a Inverse Transformation we inverse the factors.

    qDebug() << xFactor;
    qDebug() << yFactor;

    xFactor = 1 / xFactor;
    yFactor = 1 / yFactor;


    for(int i = 0; i < resizedImage.getHeight(); i++){
        for(int j = 0; j < resizedImage.getWidth(); j++){
            int originalImageXIndex = floor(j * xFactor);
            int originalImageYIndex = floor(i * yFactor);
            QRgb colorValue = this -> image.getImage().pixel(originalImageXIndex, originalImageYIndex);
            resizedImage.getImage().setPixel(j,i,colorValue);
        }
    }

    emit imageChanged(resizedImage);
}

void Scaling::bilinealInterpolation(Image& resizedImage, float xFactor, float yFactor)
{

    xFactor = 1 / xFactor;
    yFactor = 1 / yFactor;


    for(int i = 0; i < resizedImage.getHeight(); i++){
        for(int j = 0; j < resizedImage.getWidth(); j++){
            int smallestX = floor(j * xFactor);
            int biggestX = ceil(j * xFactor);

            int smallestY = floor(i * yFactor);
            int biggestY = ceil(i * yFactor);

            QRgb leftmostUpperColorValue = this -> image.getImage().pixel(smallestX, smallestY);
            QRgb rightmostUpperColorValue = this -> image.getImage().pixel(biggestX, smallestY);
            QRgb leftmostLowerColorValue = this -> image.getImage().pixel(smallestX, biggestY);
            QRgb rightmostLowerColorValue = this -> image.getImage().pixel(biggestX, biggestY);

            float xOffset = (j * xFactor) - smallestX;
            float yOffset = (i * yFactor) - smallestY;

            QColor horizontalValue = sumColors(leftmostUpperColorValue,
                                            multiplyColorWithConstant(
                                                substractColors(rightmostUpperColorValue,leftmostUpperColorValue),
                                                xOffset)
                                            );
            QColor verticalValue = sumColors(leftmostLowerColorValue,
                                            multiplyColorWithConstant(
                                                substractColors(rightmostLowerColorValue,leftmostLowerColorValue),
                                                xOffset)
                                            );

            QColor colorValue = sumColors(horizontalValue,
                                            multiplyColorWithConstant(
                                                substractColors(verticalValue,horizontalValue),
                                                yOffset)
                                            );

            resizedImage.getImage().setPixel(j,i,colorValue.rgb());
        }
    }

    emit imageChanged(resizedImage);

}

QColor Scaling::sumColors(QColor colorA, QColor colorB)
{

    int red = colorA.red() + colorB.red();
    int blue = colorA.blue() + colorB.blue();
    int green = colorA.green() + colorB.green();

    return QColor(red, blue, green);

}

QColor Scaling::substractColors(QColor colorA, QColor colorB)
{

    int red = colorA.red() - colorB.red();
    int blue = colorA.blue() - colorB.blue();
    int green = colorA.green() - colorB.green();

    return QColor(red, blue, green, 1);

}

QColor Scaling::multiplyColorWithConstant(QColor colorA, float constant)
{
    int red = colorA.red() * constant;
    int blue = colorA.blue() * constant;
    int green = colorA.green()  * constant;

    return QColor(red, blue, green, 1);
}


