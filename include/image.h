#ifndef IMAGE_H
#define IMAGE_H

#include <QString>
#include <QImage>

class Image
{
public:
    Image();
    Image(const QString& filepath);

    bool loadImage(const QString& filepath);
    bool saveImage(const QString& filepath, const char* format, int quality);

    QImage image() const;
    void setImage(const QImage &image);

    QString filePath() const;

    QString fileFormat() const;

public:
    QImage mImage;
    QString mFilePath;
    QString mFileFormat;

};

#endif // IMAGE_H
