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

void Rotation::inverseTransform(float rotationAngle)
{
    std::vector<std::vector<float>> rotationMatrix = generateRotationMatrix(rotationAngle);

    int width = this -> image.getWidth();
    int height = this -> image.getHeight();

    int leftMostUpperIndex[2] =  {0,0};
    int leftMostLowerIndex[2] =  {int(height * (-1) * rotationMatrix[0][1]), int(height  * rotationMatrix[1][1])};
    int rightMostUpperIndex[2] = {int(width * rotationMatrix[0][0]), int(width * rotationMatrix[1][0])};
    int rightMostLowerIndex[2] = {int(width * rotationMatrix[0][0]) - int(height * rotationMatrix[0][1]),
                                  int(width * rotationMatrix[1][0]) + int(height * rotationMatrix[1][1])};

    int originalLeftMostUpperIndex[2] = {0,0};
    int originalRightMostUpperIndex[2] = {image.getWidth(),0};
    int originalLeftMostLowerIndex[2] = {0, image.getHeight()};
    int originalRightMostLowerIndex[2] = {image.getWidth(), image.getHeight()};

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

            int rotatedPoint[2] = {rotatedXCoordinate, rotatedYCoordinate};
            QRgb colorValue;

            if(pointIsInsideImage(originalLeftMostUpperIndex, originalRightMostUpperIndex,
                                  originalLeftMostLowerIndex, originalRightMostLowerIndex,
                                  rotatedPoint)){
                colorValue = this -> image.getImage().pixel(rotatedXCoordinate, rotatedYCoordinate);
            }else{
                colorValue = this -> image.getBackgroundColor().rgb();
            }



            rotatedImage.getImage().setPixel(j,i,colorValue);
        }
    }


    int backgroundPixels = abs((newWidth * newHeight) - calculateParallelogramArea(leftMostUpperIndex, rightMostUpperIndex,
                                                     leftMostLowerIndex, rightMostLowerIndex));
    qDebug() << "Background pixels: " << backgroundPixels;
    rotatedImage.setBackgroundPixels(backgroundPixels);
    emit imageChanged(rotatedImage);
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
    for(int i = 0; i < this -> image.getHeight(); i++){
        for(int j = 0; j < this -> image.getWidth(); j++){

            int rotatedXCoordinate = (j * rotationMatrix[0][0]) - (i * rotationMatrix[0][1]);
            int rotatedYCoordinate = (j * rotationMatrix[0][1]) + (i * rotationMatrix[1][1]);

            rotatedXCoordinate -= minWidth;
            rotatedYCoordinate -= minHeight;

            QRgb colorValue = this -> image.getImage().pixel(j,i);

            rotatedImage.getImage().setPixel(rotatedXCoordinate, rotatedYCoordinate,colorValue);

        }
    }

    int backgroundPixels = abs((newWidth * newHeight) - calculateParallelogramArea(leftMostUpperIndex, rightMostUpperIndex,
                                                     leftMostLowerIndex, rightMostLowerIndex));
    qDebug() << "Background pixels: " << backgroundPixels;
    rotatedImage.setBackgroundPixels(backgroundPixels);
    emit imageChanged(rotatedImage);
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

int Rotation::calculateParallelogramArea(int leftmostUpperCorner[], int rightmostUpperCorner[],
                                        int leftmostLowerCorner[], int rightmostLowerCorner[])
{

    return ((rightmostUpperCorner[0] - leftmostUpperCorner[0])*(rightmostLowerCorner[1] - leftmostUpperCorner[1]))
            - ((rightmostUpperCorner[1] - leftmostUpperCorner[1])*(rightmostLowerCorner[0] - leftmostUpperCorner[0]));
}

bool Rotation::pointIsInsideImage(int leftmostUpperCorner[], int rightmostUpperCorner[],
                                  int leftmostLowerCorner[], int rightmostLowerCorner[],
                                  int rotatedPoint[])
{
    int firstTriangleArea = calculateTriangleArea(leftmostLowerCorner, leftmostUpperCorner, rotatedPoint);
    int secondTriangleArea = calculateTriangleArea(leftmostUpperCorner, rightmostUpperCorner, rotatedPoint);
    int thirdTriangleArea = calculateTriangleArea(rightmostLowerCorner, rightmostUpperCorner, rotatedPoint);
    int fourthTriangleArea = calculateTriangleArea(leftmostLowerCorner, rightmostLowerCorner, rotatedPoint);

    int parallelogramArea = calculateParallelogramArea(leftmostUpperCorner, rightmostUpperCorner,
                                                       leftmostLowerCorner, rightmostLowerCorner);

//    qDebug() << "Triangle's sum area: " << (firstTriangleArea + secondTriangleArea + thirdTriangleArea + fourthTriangleArea);
//    qDebug() << "Parallelogram area: " << parallelogramArea;

    if((firstTriangleArea + secondTriangleArea + thirdTriangleArea + fourthTriangleArea) > parallelogramArea){
        return false;
    }else {
        if(firstTriangleArea == 0 || secondTriangleArea == 0 || thirdTriangleArea == 0 || fourthTriangleArea == 0)
            return false;
    }
        return true;
}

int Rotation::calculateTriangleArea(int a[], int b[], int c[])
{
    int area = abs( ((a[0]-c[0])*(b[1]-a[1]))
                  - ((a[0]-b[0])*(c[1]-a[1]))
                   );
    return area/2;
}
