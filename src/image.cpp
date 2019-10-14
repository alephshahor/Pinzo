#include "../include/image.h"
#include <QImageReader>
#include <QMessageBox>
#include <QGuiApplication>
#include <QDir>
#include <QObject>

Image::Image():
    mImage()
{

}

Image::Image(const QString &filepath):
    mImage()
{
    loadImage(filepath);
}

bool Image::loadImage(const QString &filepath)
{
    QImageReader reader(filepath);
    reader.setAutoTransform(true);
    mImage = reader.read();
    if(mImage.isNull()){
        return false;
    }else return true;
}

bool Image::saveImage(const QString &filepath, const char* format, int quality)
{
    return mImage.save(filepath, format, quality);
}

QImage Image::image() const
{
    return mImage;
}

void Image::setImage(const QImage &image)
{
    mImage = image;
}
