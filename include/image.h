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
    QString fileName() const;
    QString fileFormat() const;
    int fileDepth() const;
    bool isGray() const;
    std::pair<int, int> fileDimension() const;


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
