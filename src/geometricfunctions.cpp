#include "include/geometricfunctions.h"

GeometricFunctions::GeometricFunctions(Image image):
    image(image)
{

}

Image GeometricFunctions::verticalMirror()
{
    Image mirroredImage(this -> image);
    for(int i = this -> image.getHeight() - 1; i >= 0 ; i--){
        for(int j = 0; j < this -> image.getWidth(); j++){
            QRgb colorValue = this -> image.getImage().pixel(j,i);
            mirroredImage.getImage().setPixel(j, this -> image.getHeight() - i - 1, colorValue);
        }
    }

    return mirroredImage;
}

Image GeometricFunctions::horizontalMirror()
{
    Image mirroredImage(this -> image);
    for(int i = 0; i < this -> image.getHeight(); i++){
        for(int j = this -> image.getWidth() - 1; j >= 0 ; j--){
            QRgb colorValue = this -> image.getImage().pixel(j,i);
            mirroredImage.getImage().setPixel(this -> image.getWidth() -1 - j, i , colorValue);
        }
    }

    return mirroredImage;
}

Image GeometricFunctions::traspose()
{
    Image mirroredImage(this -> image, this -> image.getHeight(), this -> image.getWidth());
    for(int i = 0; i < this -> image.getHeight(); i++){
        for(int j = 0; j < this -> image.getWidth(); j++){
            QRgb colorValue = this -> image.getImage().pixel(j,i);
            mirroredImage.getImage().setPixel(i,j,colorValue);
        }
    }

    return mirroredImage;
}

Image GeometricFunctions::rotation(std::string direction)
{
    Image mirroredImage(this -> image, this -> image.getHeight(), this -> image.getWidth());
    for(int i = 0; i < this -> image.getHeight(); i++){
        for(int j = 0; j < this -> image.getWidth(); j++){
            QRgb colorValue = this -> image.getImage().pixel(j,i);
            if(direction == "clockwise")
               mirroredImage.getImage().setPixel(image.getHeight() - i - 1,j,colorValue);
            else
               mirroredImage.getImage().setPixel(i,image.getWidth() - 1 - j,colorValue);
        }
    }

    return mirroredImage;
}
