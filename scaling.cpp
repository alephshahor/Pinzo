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


}


