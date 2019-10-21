#ifndef CUMULATIVEHISTOGRAM_H
#define CUMULATIVEHISTOGRAM_H

#include <QWidget>
#include "absolutehistogram.h"

namespace Ui {
class CumulativeHistogram;
}

class CumulativeHistogram : public AbsoluteHistogram
{
    Q_OBJECT

public:
    CumulativeHistogram(Image image);
    ~CumulativeHistogram() override;

    void calculateHistogramValues();
//    void createHistogram() override;
//    void displayHistogram() override;

    void calculateHistogramValues(int (*func)(QColor)) override;
private:
    Ui::CumulativeHistogram *ui;

};

#endif // CUMULATIVEHISTOGRAM_H
