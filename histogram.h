#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "include/image.h"

#include <QtCharts>

namespace Ui {
class Histogram;
}

class Histogram
{

public:
    explicit Histogram(Image image);
    Histogram(Histogram& histogram);
    virtual ~Histogram();

    Image getImage() const;
    void setImage(const Image &image);

    int getCurrentType() const;
    void setCurrentType(int value);

    QVector<double> getVPixelValue() const;

    QVector<double> getVPixelKey() const;

    int getImageRange() const;
    void setImageRange(int imageRange);

    void setVPixelValue(const QVector<double> &vPixelValue);
    void setVPixelKey(const QVector<double> &vPixelKey);




    protected:



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
    virtual void createHistogram() = 0;

    /*!
     * \brief calculateHistogramKeys it calculates the keys of
     * the histogram, that means the values of the x-axis.
     */
    void calculateHistogramKeys();

    /*!
     * \brief displayHistogram it creates and display the histogram
     * according to the current type of visualization.
     */
    virtual void displayHistogram() = 0;


protected:

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

};

#endif // HISTOGRAM_H
