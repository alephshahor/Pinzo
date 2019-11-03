#include "histogramspecification.h"
#include "ui_histogramspecification.h"

#include <cmath>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "histogram.h"
#include "absolutehistogram.h"


HistogramSpecification::HistogramSpecification(Image& image, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistogramSpecification),
    mOriginalImage(image)
{
    ui->setupUi(this);
    connect(ui -> loadRefImgButton, &QPushButton::clicked,
            this, &HistogramSpecification::loadReferenceImage);
    connect(ui -> applyButton, &QPushButton::clicked,
            this, &HistogramSpecification::processReferenceHistogram);
}

HistogramSpecification::~HistogramSpecification()
{
    delete ui;
}

QVector<double> HistogramSpecification::matchHistograms(int (*func)(QColor))
{
    Histogram* originalHistogram = new AbsoluteHistogram(mOriginalImage, nullptr);
    Histogram* referenceHistogram = new AbsoluteHistogram(mReferenceImage, nullptr);

    QVector<double> originalDistributionFun = originalHistogram -> calculateDistributionFunction(func);
    QVector<double> referenceDistributionFun = referenceHistogram -> calculateDistributionFunction(func);


    int nIntensityValues = pow(2,mOriginalImage.getImageDepth());

    QVector<double> mappingFunction(nIntensityValues);

    for(int i = 0; i < nIntensityValues; i++){
        int j = nIntensityValues - 1;
        do{
            mappingFunction[i] = j;
            j -= 1;
        }while(j >= 0 && originalDistributionFun[i] <= referenceDistributionFun[j]);
    }

    return mappingFunction;
}

void HistogramSpecification::applyMappingFunction( QVector<double>& rMappingFunc, QVector<double>& gMappingFunc, QVector<double>& bMappingFunc)
{
    QRgb *st = (QRgb *) mOriginalImage.getImage().bits();
    quint64 pixelCount = mOriginalImage.getWidth() * mOriginalImage.getHeight();

    for (quint64 p = 0; p < pixelCount; p++) {
        int red = rMappingFunc[qRed(st[p])];
        int green = gMappingFunc[qGreen(st[p])];
        int blue = bMappingFunc[qBlue(st[p])];

        st[p] = QColor(red, green, blue).rgb();
    }
}

void HistogramSpecification::processReferenceHistogram()
{
    if(ui -> equalizationCheckbox -> checkState() == false){
        if(mReferenceImage.getWidth() == 0){ // No image has been loaded.
            QMessageBox messageBox;
            messageBox.critical(0, "Error", "You need to specify a reference image.");
            messageBox.setFixedSize(500,200);
        }else{
            QVector<double> rMappingFunc = matchHistograms(Histogram::calculateRedColorValue);
            QVector<double> gMappingFunc = matchHistograms(Histogram::calculateGreenColorValue);
            QVector<double> bMappingFunc = matchHistograms(Histogram::calculateBlueColorValue);
            applyMappingFunction(rMappingFunc, gMappingFunc, bMappingFunc);
            emit imageChanged();
        }
    }
}

void HistogramSpecification::loadReferenceImage()
{
    QString filepath = QFileDialog::getOpenFileName((this), "Open the file");
    mReferenceImage = Image(filepath);
    ui -> refImgName -> setText(mReferenceImage.getImageName());
}
