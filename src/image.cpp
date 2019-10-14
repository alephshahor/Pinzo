#include "../include/image.h"
#include <QImageReader>
#include <QMessageBox>
#include <QGuiApplication>
#include <QDir>
#include <QObject>
#include <QDebug>
#include <QRegExp>

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
    }else{
        QRegExp reg("^(/|\\w+)+.(\\w+)");
        if(reg.exactMatch(filepath)){
            mFileFormat = reg.cap(2);
        }
        mFilePath = filepath;
        return true;
    }
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

QString Image::fileFormat() const
{
    return mFileFormat;
}

QString Image::filePath() const
{
    return mFilePath;
}


