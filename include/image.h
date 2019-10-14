#ifndef IMAGE_H
#define IMAGE_H

#include <QString>
#include <QImage>

class Image
{
public:
    Image();
    bool loadImage(const QString& filepath);
    bool saveImage(const QString& filepath, const char* format, int quality);
    QImage image() const;
    void setImage(const QImage &image);

public:
    QImage mImage;

};

#endif // IMAGE_H
