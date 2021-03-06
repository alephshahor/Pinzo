#ifndef IMAGE_H
#define IMAGE_H

#include <QString>
#include <QImage>

class Image
{
public:
    Image();
    Image(const QString& filepath);
    Image(Image image, QRect rect);
    Image(Image& image);
    Image(Image &image, int width, int height);

    bool loadImage(const QString& filepath);
    bool saveImage(const QString& filepath, const char* format, int quality);

    QImage& getImage();
    void setImage(const QImage &image);

    QString getImagePath() const;
    QString getImageName() const;
    QString getImageFormat() const;

    int getNumberOfIntensityValues() const;
    int getImageDepth() const;
    bool isGray() const;
    std::pair<int, int> getImageDimension() const;
    int getWidth();
    int getHeight();

public:
    QImage mImage;
    QString mFileName;
    QString mFilePath;
    QString mFileFormat;
    int     mFileDepth;
    bool    mIsGray;
    std::pair<int,int> mFileDimension;



};

#endif // IMAGE_H
