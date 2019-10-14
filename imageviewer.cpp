#include "imageviewer.h"
#include "ui_imageviewer.h"

ImageViewer::ImageViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageViewer),
    mImagePixMap(),
    mImage()
{

    ui->setupUi(this);
    scaleImage();
}

ImageViewer::~ImageViewer()
{
    delete ui;
}

Image ImageViewer::image() const
{
    return mImage;
}

void ImageViewer::setImage(const Image &image)
{
    mImage = image;
    mImagePixMap.convertFromImage(mImage.image());
    ui -> imageLabel -> setPixmap(mImagePixMap);
}

void ImageViewer::scaleImage()
{
    ui -> imageLabel -> setScaledContents( true );
    ui -> imageLabel -> setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
}
