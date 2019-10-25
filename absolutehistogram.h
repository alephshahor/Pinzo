#ifndef ABSOLUTEHISTOGRAM_H
#define ABSOLUTEHISTOGRAM_H

#include <QWidget>
#include "histogram.h"
#include "include/image.h"


class AbsoluteHistogram : public Histogram
{
    Q_OBJECT

public:

    AbsoluteHistogram(Image image, QWidget *parent);
    AbsoluteHistogram(AbsoluteHistogram& histogram);
    ~AbsoluteHistogram() override;


private:

    /*!
     * \brief calculateHistogramValues it calculates the values of
     * the histogram, that means the values of the y-axis.
     */
    void calculateHistogramValues(int(*func)(QColor)) override;


    /*!
     * \brief displayInfo it refreshes the values of the QLabels
     * that stores the information of the histogram.
     */
    void displayInfo() override;

};

#endif // ABSOLUTEHISTOGRAM_H
