#include "rotation.h"
#include "ui_rotation.h"

#include <math.h>
#include <algorithm>

#include <QDebug>

#define PI 3.14159265

Rotation::Rotation(Image image, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Rotation),
    image(image)
{
    ui->setupUi(this);
    connect(ui -> angleText, &QLineEdit::returnPressed,
            this, &Rotation::rotateImage);
}

Rotation::~Rotation()
{
    delete ui;
}

void Rotation::rotateImage()
{
    float rotationAngle = ui -> angleText -> text().toFloat();
    switch(ui -> rotationComboBox->currentIndex()){
        case directTransformationRotation:
            directTransform(rotationAngle);
        break;

        case inverseTransformationRotation:
            inverseTransform(rotationAngle);
        break;
    }
}

void Rotation::directTransform(float rotationAngle)
{
    std::vector<std::vector<float>> rotationMatrix = generateRotationMatrix(rotationAngle);

    int width = this -> image.getWidth();
    int height = this -> image.getHeight();

    int leftMostUpperIndex[2] =  {0,0};
    int leftMostLowerIndex[2] =  {int(height * (-1) * rotationMatrix[0][1]), int(height  * rotationMatrix[1][1])};
    int rightMostUpperIndex[2] = {int(width * rotationMatrix[0][0]), int(width * rotationMatrix[1][0])};
    int rightMostLowerIndex[2] = {int(width * rotationMatrix[0][0]) - int(height * rotationMatrix[0][1]),
                                  int(width * rotationMatrix[1][0]) + int(height * rotationMatrix[1][1])};

    qDebug() << "LMU" << leftMostUpperIndex[0] << leftMostUpperIndex[1];
    qDebug() << "LML" << leftMostLowerIndex[0] << leftMostLowerIndex[1];
    qDebug() << "RMU" << rightMostUpperIndex[0] << rightMostUpperIndex[1];
    qDebug() << "RML" << rightMostLowerIndex[0] << rightMostLowerIndex[1];

    std::vector<int> indexes = {leftMostUpperIndex[0], leftMostLowerIndex[0], rightMostLowerIndex[0], rightMostUpperIndex[0]};
    int maxWidth = maxIndex(indexes);
    int minWidth = minIndex(indexes);

    indexes = {leftMostUpperIndex[1], leftMostLowerIndex[1], rightMostLowerIndex[1], rightMostUpperIndex[1]};
    int maxHeight = maxIndex(indexes);
    int minHeight = minIndex(indexes);

    int newWidth = abs(maxWidth - minWidth);
    int newHeight = abs(maxHeight - minHeight);

    qDebug() << "Cos theta" << rotationMatrix[0][0];
    qDebug() << "Sin theta" << rotationMatrix[0][1];
    qDebug() << "Sin theta" << rotationMatrix[1][0];
    qDebug() << "Cos theta" << rotationMatrix[1][1];
    qDebug() << "Min width: " << minWidth;
    qDebug() << "Max width: " << maxWidth;
    qDebug() << "Min height: " << minHeight;
    qDebug() << "Max height: " << maxHeight;

    Image rotatedImage(this -> image,newWidth,newHeight);
    for(int i = 0; i < rotatedImage.getHeight(); i++){
        for(int j = 0; j < rotatedImage.getWidth(); j++){
            int jAsCoordinate = j + minWidth;
            int iAsCoordinate = i + minHeight;

            int rotatedXCoordinate = (jAsCoordinate * rotationMatrix[0][0]) + (iAsCoordinate * rotationMatrix[0][1]);
            int rotatedYCoordinate = (-1 * jAsCoordinate * rotationMatrix[0][1]) + (iAsCoordinate * rotationMatrix[1][1]);
            QRgb colorValue = this -> image.getImage().pixel(rotatedXCoordinate, rotatedYCoordinate);
            rotatedImage.getImage().setPixel(j,i,colorValue);
        }
    }

    emit imageChanged(rotatedImage);
}

void Rotation::inverseTransform(float rotationAngle)
{

}
std::vector<std::vector<float> > Rotation::generateRotationMatrix(float rotationAngle)
{
    std::vector<std::vector<float>> rotationMatrix;

    std::vector<float> firstRow;
    firstRow.push_back(cos(rotationAngle * PI/180));
    firstRow.push_back(sin(rotationAngle * PI/180));

    std::vector<float> secondRow;
    secondRow.push_back(sin(rotationAngle * PI/180));
    secondRow.push_back(cos(rotationAngle * PI/180));

    rotationMatrix.push_back(firstRow);
    rotationMatrix.push_back(secondRow);

    return rotationMatrix;
}


int Rotation::maxIndex(std::vector<int> indexes)
{
    int biggestIndex = indexes[0];
    for(auto index : indexes){
        if(index > biggestIndex){
            biggestIndex = index;
        }
    }
    return biggestIndex;
}

int Rotation::minIndex(std::vector<int> indexes)
{
    int smallestIndex = indexes[0];
    for(auto index : indexes){
        if(index < smallestIndex){
            smallestIndex = index;
        }
    }
    return smallestIndex;
}
