#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "include/image.h"
#include "include/qcustomplot.h"

#include <QWidget>
#include <QtCharts>

namespace Ui {
class Histogram;
}

class Histogram : public QWidget
{
    Q_OBJECT

public:
    explicit Histogram(Image image, QWidget *parent = nullptr);
    Histogram(Histogram& histogram);
    ~Histogram();

    Image getImage() const;
    void setImage(const Image &image);

    int getCurrentType() const;
    void setCurrentType(int value);

    QVector<double> getVPixelValue() const;

    QVector<double> getVPixelKey() const;

private:

    /*!
     * \brief calculateModeValue calculates the mode of the
     * histogram.
     * \return the value of the mode.
     */
    int calculateModeValue();

    /*!
     * \brief calculateModeFrequency calculates the frequency
     * of the mode value in the histogram.
     * \return the value of the frequency.
     */
    int calculateModeFrequency();

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
     * \brief numberOfPixels of the image.
     * \return the number of pixels.
     */
    int numberOfPixels();

    /*!
     * \brief displayInfo it refreshes the values of the QLabels
     * that stores the information of the histogram.
     */
    void displayInfo();

    // Setters
    void setVPixelValue(const QVector<double> &vPixelValue);
    void setVPixelKey(const QVector<double> &vPixelKey);

    /*!
     * \brief createHistogram it creates a QCustomPlot graph.
     */
    void createHistogram();

    /*!
     * \brief calculateHistogramKeys it calculates the keys of
     * the histogram, that means the values of the x-axis.
     */
    void calculateHistogramKeys();

    /*!
     * \brief calculateHistogramValues it calculates the values of
     * the histogram, that means the values of the y-axis.
     */
    void calculateHistogramValues(int(*func)(QColor));

    /*!
     * \brief displayHistogram it creates and display the histogram
     * according to the current type of visualization.
     */
    void displayHistogram();

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

    Ui::Histogram *ui;
    /*!
     * \brief mImage the image the histogram takes to analyze.
     */
    Image mImage;

    /*!
     * \brief The mDisplayType enum the types of histogram
     * visualization.
     */
    enum mDisplayType {Lightness, Red, Green, Blue};

    /*!
     * \brief mCurrentType stores the current visualization type.
     */
    int mCurrentType;

    /*!
     * \brief mVPixelValue the y-axis values.
     */
    QVector<double> mVPixelValue;

    /*!
     * \brief mVPixelKey the x-axis values.
     */
    QVector<double> mVPixelKey;

    /*!
     * \brief mImageRange number of possible intensity values
     * depending on the image depth.
     */
    int mImageRange;

};

#endif // HISTOGRAM_H
