#include "histogramspecification.h"
#include "ui_histogramspecification.h"

#include <cmath>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "histogram.h"
#include "absolutehistogram.h"
#include "cumulativehistogram.h"


HistogramSpecification::HistogramSpecification(Image& image, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistogramSpecification),
    mOriginalImage(image)
{
    ui->setupUi(this);
    connect(ui -> loadRefImgButton, &QPushButton::clicked,
            this, &HistogramSpecification::loadReferenceImage);
    connect(ui -> applyButton, &QPushButton::clicked,
            this, &HistogramSpecification::processHistogramSpecification);
    connect(ui -> equalizationCheckbox, &QCheckBox::clicked,
            this, &HistogramSpecification::processEqualizationCheckbox);

    ui -> equalizationCheckbox -> setChecked(false);
    ui -> equalizationCheckbox -> setCheckState(Qt::Unchecked);
    ui -> refFuncComboBox -> setDisabled(true);
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

void HistogramSpecification::processHistogramSpecification()
{
    if(ui -> equalizationCheckbox -> checkState() == Qt::Unchecked){
        if(mReferenceImage.getWidth() == 0){ // No image has been loaded.
            QMessageBox messageBox;
            messageBox.critical(0, "Error", "You need to specify a reference image.");
            messageBox.setFixedSize(500,200);
        }else{
            processReferenceHistogram();
        }
    }else processEqualizationFunction();
}

void HistogramSpecification::processReferenceHistogram()
{

    QVector<double> rMappingFunc = matchHistograms(Histogram::calculateRedColorValue);
    QVector<double> gMappingFunc = matchHistograms(Histogram::calculateGreenColorValue);
    QVector<double> bMappingFunc = matchHistograms(Histogram::calculateBlueColorValue);
    applyMappingFunction(rMappingFunc, gMappingFunc, bMappingFunc);
    emit imageChanged();
}

void HistogramSpecification::loadReferenceImage()
{
    QString filepath = QFileDialog::getOpenFileName((this), "Open the file");
    mReferenceImage = Image(filepath);
    ui -> refImgName -> setText(mReferenceImage.getImageName());
}

void HistogramSpecification::processEqualizationCheckbox()
{
    if(ui -> equalizationCheckbox -> checkState() == Qt::Checked){
        ui -> refImgName -> setDisabled(true);
        ui -> loadRefImgButton -> setDisabled(true);
        ui -> refFuncComboBox -> setDisabled(false);
    }else{
        ui -> refImgName -> setDisabled(false);
        ui -> loadRefImgButton -> setDisabled(false);
        ui -> refFuncComboBox -> setDisabled(true);
    }
}

void HistogramSpecification::processEqualizationFunction()
{
    int functionIndex = ui -> refFuncComboBox -> currentIndex();
    QVector<double> rEqualizationFunction;
    QVector<double> gEqualizationFunction;
    QVector<double> bEqualizationFunction;
    switch(functionIndex){
        case Linear:
            rEqualizationFunction = getLinearEqualizationFunction(Histogram::calculateRedColorValue);
            gEqualizationFunction = getLinearEqualizationFunction(Histogram::calculateGreenColorValue);
            bEqualizationFunction = getLinearEqualizationFunction(Histogram::calculateBlueColorValue);
        break;
    }
    applyMappingFunction(rEqualizationFunction, gEqualizationFunction, bEqualizationFunction);
    emit imageChanged();
}

QVector<double> HistogramSpecification::getLinearEqualizationFunction(int (*func)(QColor))
{
    Histogram* originalHistogram = new CumulativeHistogram(mOriginalImage, nullptr);
    originalHistogram -> calculateHistogramValues(func);

    int nIntensityValues = pow(2,mOriginalImage.getImageDepth());
    int width = mOriginalImage.getWidth();
    int height = mOriginalImage.getHeight();
    int totalPixels = width * height;

    QVector<double> mappingFunction(nIntensityValues);

    for(int i = 0; i < nIntensityValues; i++){
        int newValue = originalHistogram -> getValue(i) * (nIntensityValues - 1) / totalPixels;
        mappingFunction[i] = newValue;
    }

    return mappingFunction;
}
