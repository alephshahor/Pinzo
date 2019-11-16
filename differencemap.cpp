#include "differencemap.h"
#include "ui_differencemap.h"

#include <QFileDialog>

DifferenceMap::DifferenceMap(Image image, QWidget *parent) :
    QWidget(parent),
    mOriginalImage(image),
    ui(new Ui::DifferenceMap)
{
    ui->setupUi(this);
    ui -> thresholdSlider -> setMinimum(0);
    ui -> thresholdSlider -> setMaximum(mOriginalImage.getNumberOfIntensityValues() - 1);
    connect(ui -> thresholdSlider, &QSlider::valueChanged,
            this, &DifferenceMap::processThresholdSlider);
    connect(ui -> loadRefImgButton, &QPushButton::clicked,
            this, &DifferenceMap::loadReferenceImage);
}

DifferenceMap::~DifferenceMap()
{
    delete ui;
}

void DifferenceMap::loadReferenceImage()
{
    QString filepath = QFileDialog::getOpenFileName((this), "Open the file");
    mReferenceImage = Image(filepath);
    ui -> refImgName -> setText(mReferenceImage.getImageName());
}

void DifferenceMap::processThresholdSlider(){
    int threshold = ui -> thresholdSlider -> value();
    mThreshold = threshold;
    ui -> thresholdText -> setText(QString::number(threshold));
    calculateDifferenceMap();
}

void DifferenceMap::calculateDifferenceMap(){
    ImageDifference imageDifference(mOriginalImage, nullptr);
    imageDifference.setReferenceImage(mReferenceImage);
    Image differenceImage;
    differenceImage = imageDifference.calculateDifference_();

    QRgb *st = (QRgb *) differenceImage.getImage().bits();
    quint64 pixelCount = differenceImage.getWidth() * differenceImage.getHeight();

    for (quint64 p = 0; p < pixelCount; p++) {

        int red = 255;
        int green = 0;
        int blue = 0;

        if(qRed(st[p])  <= mThreshold)
            red = qRed(st[p]);
        if(qGreen(st[p]) <= mThreshold)
            green = qGreen(st[p]);
        if(qBlue(st[p]) <= mThreshold)
            blue = qBlue(st[p]);
        st[p] = QColor(red, green, blue).rgb();
    }
    emit imageChanged(differenceImage);
}
