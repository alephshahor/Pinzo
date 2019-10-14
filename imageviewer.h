#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "include/image.h"
#include <QWidget>


namespace Ui {
class ImageViewer;
}

class ImageViewer : public QWidget
{
    Q_OBJECT

public:

    explicit ImageViewer(QWidget *parent = nullptr);
    ~ImageViewer();

    /*!
     * \brief  mImage's getter.
     * \return mImage.
     */
    Image image() const;

    /*!
     * \brief sets mImage and mImagePixmap attributes.
     * \param image
     */
    void setImage(const Image &image);

private:

    /*!
     * \brief scales and sets the size policy of the
     * ui's QLabel's element.
     */
    void scaleImage();

private:

    Ui::ImageViewer *ui;

    /*!
     * \brief mImagePixMap is a QPixmap needed to
     * represent the image in the GUI.
     */
    QPixmap mImagePixMap;
    Image mImage;
};

#endif // IMAGEVIEWER_H
