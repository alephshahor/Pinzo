#include "../include/image.h"
#include "../include/util.h"

#include <QImageReader>
#include <QMessageBox>
#include <QGuiApplication>
#include <QDir>
#include <QObject>
#include <QDebug>
#include <QRegExp>
#include <cmath>

Image::Image():
    mImage()
{

}

Image::Image(const QString &filepath):
    mImage()
{
    loadImage(filepath);
}

Image::Image(Image image, QRect rect):
    mImage(),
    mFileName(image.getImageName()),
    mFilePath(image.getImagePath()),
    mFileFormat(image.getImageFormat()),
    mFileDepth(image.getImageDepth()),
    mIsGray(image.isGray()),
    mFileDimension(std::make_pair(rect.width(), rect.height()))
{
    mImage = image.getImage().copy(rect);
}

Image::Image(Image &image):
    mImage(image.getImage()),
    mFileName(image.getImageName()),
    mFilePath(image.getImagePath()),
    mFileFormat(image.getImageFormat()),
    mFileDepth(image.getImageDepth()),
    mIsGray(image.isGray()),
    mFileDimension(image.getImageDimension())
{
}

bool Image::loadImage(const QString &filepath)
{
    QImageReader reader(filepath);
    reader.setAutoTransform(true);
    mImage = reader.read();
    if(mImage.isNull()){
        return false;
    }else{
            QString fileNameCommand("identify -format \"%t\" " + filepath);
            const char* fileNameCommand_ = fileNameCommand.toLocal8Bit().data();
            mFileName = QString::fromUtf8(executeCommand(fileNameCommand_).c_str());

            QString fileFormatCommand("identify -format \"%m\" " + filepath);
            const char* fileFormatCommand_ = fileFormatCommand.toLocal8Bit().data();
            mFileFormat = QString::fromUtf8(executeCommand(fileFormatCommand_).c_str());

            if(mFileFormat == "TIFF"){
                mImage = mImage.convertToFormat(QImage::Format_RGB32, Qt::AutoColor);
            }

            QString fileDimensionCommand("identify -format \"%w %h\" " + filepath);
            const char* fileDimensionCommand_ = fileDimensionCommand.toLocal8Bit().data();
            QString fileDimension = QString::fromUtf8(executeCommand(fileDimensionCommand_).c_str());
            mFileDimension.first  = fileDimension.split(QRegExp("\\s+"), QString::SkipEmptyParts).first().toInt();
            mFileDimension.second = fileDimension.split(QRegExp("\\s+"), QString::SkipEmptyParts).last().toInt();

            QString fileDepthCommand("identify -format \"%z\" " + filepath);
            const char* fileDepthCommand_ = fileDepthCommand.toLocal8Bit().data();
            mFileDepth = QString::fromUtf8(executeCommand(fileDepthCommand_).c_str()).toInt();

            QString currentDirectory = CURRENT_DIRECTORY;
            QString fileColorCommand("python " + currentDirectory + "/src/isGrayscale.py " + filepath);
            const char* fileColorCommand_ = fileColorCommand.toLocal8Bit().data();
            QString fileColor = QString::fromUtf8(executeCommand(fileColorCommand_).c_str());

            if(fileColor == "grayscale\n"){
                mIsGray = true;
            }else mIsGray = false;

            mFilePath = filepath;


            return true;
    }

}

bool Image::saveImage(const QString &filepath, const char* format, int quality)
{
    return mImage.save(filepath, format, quality);
}

QImage& Image::getImage()
{
    return mImage;
}

void Image::setImage(const QImage &image)
{
    mImage = image;
}

QString Image::getImageFormat() const
{
    return mFileFormat;
}

int Image::getNumberOfIntensityValues() const
{
    return pow(2, mFileDepth);
}

std::pair<int, int> Image::getImageDimension() const
{
    return mFileDimension;
}

int Image::getWidth()
{
    return mImage.width();
}

int Image::getHeight()
{
    return mImage.height();
}

bool Image::isGray() const
{
    return mIsGray;
}

int Image::getImageDepth() const
{
    return mFileDepth;
}

QString Image::getImageName() const
{
    return mFileName;
}

QString Image::getImagePath() const
{
    return mFilePath;
}


