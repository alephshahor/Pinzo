#ifndef IMAGEDIFFERENCE_H
#define IMAGEDIFFERENCE_H

#include "include/image.h"
#include <QWidget>

namespace Ui {
class ImageDifference;
}

class ImageDifference : public QWidget
{
    Q_OBJECT

public:
    explicit ImageDifference(Image image, QWidget *parent);
    ~ImageDifference();

    void setReferenceImage(Image referenceImage);
    Image calculateDifference_();


private:
    void loadReferenceImage();
    void calculateDifference();

signals:
    void imageChanged(Image image);

private:
    Ui::ImageDifference *ui;
    Image mReferenceImage;
    Image mOriginalImage;
};

#endif // IMAGEDIFFERENCE_H
