#include "imagedifference.h"
#include "ui_imagedifference.h"
#include <QFileDialog>

ImageDifference::ImageDifference(Image image, QWidget *parent) :
    QWidget(parent),
    mOriginalImage(image),
    ui(new Ui::ImageDifference)
{
    ui->setupUi(this);
    connect(ui -> loadRefImgButton, &QPushButton::clicked,
            this, &ImageDifference::loadReferenceImage);
    connect(ui -> calculateButton, &QPushButton::clicked,
            this, &ImageDifference::calculateDifference);
}

ImageDifference::~ImageDifference()
{
    delete ui;
}


void ImageDifference::calculateDifference(){

    int height = std::min(mOriginalImage.getHeight(), mReferenceImage.getHeight());
    int width = std::min(mOriginalImage.getWidth(), mReferenceImage.getWidth());

    QRect imageSelection = QRect(0,0,width,height);

    /* Images get cropped so all of them are bounded to the minimum
     * height and with between the two images that are going to be differenced*/
    Image originalImageCropped(mOriginalImage, imageSelection);
    Image referenceImageCropped(mReferenceImage, imageSelection);
    Image resultImage(originalImageCropped);


    QRgb *stResult = (QRgb *) resultImage.getImage().bits();
    QRgb *stOriginal = (QRgb *) originalImageCropped.getImage().bits();
    QRgb *stReference = (QRgb *) referenceImageCropped.getImage().bits();

    quint64 pixelCount = width * height;

    for (quint64 p = 0; p < pixelCount; p++) {

        double red = abs(qRed(stOriginal[p]) - qRed(stReference[p]));
        double green = abs(qGreen(stOriginal[p]) - qGreen(stReference[p]));
        double blue = abs(qBlue(stOriginal[p]) - qBlue(stReference[p]));

        stResult[p] = QColor(red, green, blue).rgb();
    }

    emit imageChanged(resultImage);
}

void ImageDifference::loadReferenceImage()
{
    QString filepath = QFileDialog::getOpenFileName((this), "Open the file");
    mReferenceImage = Image(filepath);
    ui -> refImgName -> setText(mReferenceImage.getImageName());
}

