#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "include/image.h"
#include <QWidget>

namespace Ui {
class Histogram;
}

class Histogram : public QWidget
{

    Q_OBJECT

public:
    explicit Histogram(Image& image, QWidget *parent);
    Histogram(Histogram& histogram);
    virtual ~Histogram();

    /*!
     * \brief displayHistogram it creates and display the histogram
     * according to the current type of visualization.
     */
    void displayHistogram();

    double calculateMean();
    double calculateStdDeviation();

    /*!
     * \brief calculateHistogramValues it calculates the values of
     * the histogram, that means the values of the y-axis.
     */
    virtual void calculateHistogramValues(int(*func)(QColor)) = 0;

    virtual void deleteBackgroundResidualColor(int pixelValue) = 0;


    /*!
     * \brief calculateHistogramKeys it calculates the keys of
     * the histogram, that means the values of the x-axis.
     */
    void calculateHistogramKeys();

    static int calculateColorLightnessValue(QColor pixel);
    static int calculateRedColorValue(QColor pixel);
    static int calculateBlueColorValue(QColor pixel);
    static int calculateGreenColorValue(QColor pixel);

    QVector<double> calculateDistributionFunction(int (*func)(QColor));

    double getValue(int index);

    void fixBackgroundResidualColor(int ammountOfResidualColor);

protected:

    Image getImage();
    void setImage(const Image &image);

    int getCurrentType() const;
    void setCurrentType(int value);

    QVector<double> getVPixelValue() const;
    QVector<double> getVPixelKey() const;

    int getImageRange() const;
    void setImageRange(int imageRange);

    void setVPixelValue(const QVector<double> &vPixelValue);
    void setVPixelKey(const QVector<double> &vPixelKey);

    int calculateMin();
    int calculateMax();
    void changeDisplayType();

    /*!
     * \brief numberOfPixels of the image.
     * \return the number of pixels.
     */
    int numberOfPixels();
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
     * \brief createHistogram it creates a QCustomPlot graph.
     */
    void createHistogram(QColor color);


    /*!
     * \brief displayHistogram it creates and display the histogram
     * according to the current type of visualization.
     */
    virtual void displayInfo();

    void setTypeLabel();


protected:

    Ui::Histogram* ui;

    /*!
     * \brief mImage the image the histogram takes to analyze.
     */
    Image mImage;

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

    /*!
     * \brief The mDisplayType enum the types of histogram
     * visualization.
     */
    enum mDisplayType {Red, Green, Blue};

    /*!
     * \brief mCurrentType stores the current visualization type.
     */
    int mCurrentType;

};

#endif // HISTOGRAM_H
