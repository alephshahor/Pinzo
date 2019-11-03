#ifndef HISTOGRAMSPECIFICATION_H
#define HISTOGRAMSPECIFICATION_H

#include <QWidget>
#include "include/image.h"

namespace Ui {
class HistogramSpecification;
}

class HistogramSpecification : public QWidget
{
    Q_OBJECT

public:
    explicit HistogramSpecification(Image& image, QWidget *parent = nullptr);
    ~HistogramSpecification();
    QVector<double> matchHistograms(int (*func)(QColor));
    void applyMappingFunction(QVector<double>& rMappingFunc, QVector<double>& gMappingFunc, QVector<double>&  bMappingFunc);

signals:
    void imageChanged();

private:
    void processReferenceHistogram();
    void loadReferenceImage();

private:
    Ui::HistogramSpecification *ui;
    Image& mOriginalImage;
    Image mReferenceImage;
};

#endif // HISTOGRAMSPECIFICATION_H
