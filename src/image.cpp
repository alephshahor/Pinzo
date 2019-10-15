#include "../include/image.h"
#include "../include/util.h"

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
            QString fileNameCommand("identify -format \"%t\" " + filepath);
            const char* fileNameCommand_ = fileNameCommand.toLocal8Bit().data();
            mFileName = QString::fromUtf8(executeCommand(fileNameCommand_).c_str());

            QString fileFormatCommand("identify -format \"%m\" " + filepath);
            const char* fileFormatCommand_ = fileFormatCommand.toLocal8Bit().data();
            mFileFormat = QString::fromUtf8(executeCommand(fileFormatCommand_).c_str());

            QString fileDimensionCommand("identify -format \"%w %h\" " + filepath);
            const char* fileDimensionCommand_ = fileDimensionCommand.toLocal8Bit().data();
            QString fileDimension = QString::fromUtf8(executeCommand(fileDimensionCommand_).c_str());
            mFileDimension.first  = fileDimension.split(QRegExp("\\s+"), QString::SkipEmptyParts).first().toInt();
            mFileDimension.second = fileDimension.split(QRegExp("\\s+"), QString::SkipEmptyParts).last().toInt();

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

std::pair<int, int> Image::fileDimension() const
{
    return mFileDimension;
}

QString Image::fileName() const
{
    return mFileName;
}

QString Image::filePath() const
{
    return mFilePath;
}


