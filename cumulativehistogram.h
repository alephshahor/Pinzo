#ifndef CUMULATIVEHISTOGRAM_H
#define CUMULATIVEHISTOGRAM_H

#include <QWidget>
#include "histogram.h"


class CumulativeHistogram : public Histogram
{
    Q_OBJECT

public:
    CumulativeHistogram(Image &image, QWidget *parent);
    ~CumulativeHistogram() override;


    QVector<double> calculateDistributionFunction(int (*func)(QColor));
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

#endif // CUMULATIVEHISTOGRAM_H
