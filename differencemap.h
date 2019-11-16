#ifndef DIFFERENCEMAP_H
#define DIFFERENCEMAP_H

#include <QWidget>
#include "include/image.h"
#include "imagedifference.h"

namespace Ui {
class DifferenceMap;
}

class DifferenceMap : public QWidget
{
    Q_OBJECT

public:
    explicit DifferenceMap(Image image, QWidget *parent = nullptr);
    ~DifferenceMap();

private:
    void processThresholdSlider();
    void loadReferenceImage();
    void calculateDifferenceMap();


signals:
    void imageChanged(Image image);

private:
    Ui::DifferenceMap *ui;
    Image mOriginalImage;
    Image mReferenceImage;
    int mThreshold;

};

#endif // DIFFERENCEMAP_H
