#ifndef ABSOLUTEHISTOGRAM_H
#define ABSOLUTEHISTOGRAM_H

#include <QWidget>
#include "histogram.h"
#include "include/image.h"

namespace Ui {
class AbsoluteHistogram;
}

class AbsoluteHistogram : public QWidget, public Histogram
{
    Q_OBJECT

public:

    explicit AbsoluteHistogram(Image image, QWidget *parent = nullptr);
    AbsoluteHistogram(AbsoluteHistogram& histogram);
    ~AbsoluteHistogram() override;

    int getCurrentType() const;
    void setCurrentType(int value);



private:



    /*!
     * \brief calculateMean calculates the mean of the histogram.
     * \return the value of the mean.
     */
    double calculateMean();

    /*!
     * \brief calculateStdDeviation calculates the Standard Deviation
     * of the histogram.
     * \return the value of the std deviation.
     */
    double calculateStdDeviation();

    /*!
     * \brief calculateMin it calculates the minimum pixel intensity value which
     * frequency of appereance is bigger than zero.
     * \return the pixel intensity value.
     */
    int calculateMin();

    /*!
     * \brief calculateMax it calculates the maximum pixel intensity value which
     * frequency of appereance is bigger than zero.
     * \return the pixel intensity value.
     */
    int calculateMax();


    /*!
     * \brief displayInfo it refreshes the values of the QLabels
     * that stores the information of the histogram.
     */
    void displayInfo();


    /*!
     * \brief calculateHistogramValues it calculates the values of
     * the histogram, that means the values of the y-axis.
     */
    void calculateHistogramValues(int(*func)(QColor));


    /*!
     * \brief createHistogram it creates a QCustomPlot graph.
     */
    void createHistogram();


    /*!
     * \brief displayHistogram it creates and display the histogram
     * according to the current type of visualization.
     */
    void displayHistogram() override;

    /*!
     * \brief changeDisplayType its called when the "changeButton" is
     * pressed. It changes the type of displaying ( red / blue / green /
     * intensity .. ).
     */
    void changeDisplayType();

    /* All this functions do the same.
     * They return from the pixel depending on what its asked its
     * lightness, red, blue... */
    static int calculateColorLightnessValue(QColor pixel);
    static int calculateRedColorValue(QColor pixel);
    static int calculateBlueColorValue(QColor pixel);
    static int calculateGreenColorValue(QColor pixel);

private:

    Ui::AbsoluteHistogram *ui;

    /*!
     * \brief The mDisplayType enum the types of histogram
     * visualization.
     */
    enum mDisplayType {Lightness, Red, Green, Blue};

    /*!
     * \brief mCurrentType stores the current visualization type.
     */
    int mCurrentType;

};

#endif // ABSOLUTEHISTOGRAM_H
