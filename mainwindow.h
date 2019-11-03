#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "include/image.h"
#include "include/rubberband.h"
#include <QMainWindow>
#include <QRubberBand>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    enum HistogramType {Absolute, Cumulative};

private:

    /*!
     * \brief  mImage's getter.
     * \return mImage.
     */
    Image image();

    /*!
     * \brief sets mImage and mImagePixmap attributes.
     * \param image
     */
    void setImage(const Image &image);

    /*!
     * \brief clone Opens a new window with the current
     * image.
     */
    void cloneWindow();

    /*!
     * \brief Opens a new empty window.
     */
    void openNewWindow();
    void openNewWindow_(Image image);


    /*!
     * \brief connectSignals connects menu options
     *  with its functions.
     *
     */
    void connectSignals();
    /*!
     * \brief opens a new Image replacing the one
     * that already existed ( if existing ).
     */
    void openImage();
    void openImage_(Image image);

    /*!
     * \brief saves the current image and its modifications.
     */
    void saveImage();

    /*!
     * \brief saves a copy of the current image with another
     * name.
     */
    void saveImageAs();

    /*!
     * \brief scales and sets the size policy of the
     * ui's QLabel's element.
     */
    void scaleImageLabel();

    /*!
     * \brief eventFilter serves as an intermediary function to
     * override the behaviour of the centralwidget so the cursor
     * position can be obtained.
     * \param obj
     * \param event
     * \return true if the object passed as parameter is a
     * centralwidget and if the Event registered is a MouseMove
     * event.
     */
    bool eventFilter(QObject* obj, QEvent* event);

    /*!
     * \brief displays the position of the cursor with respect to
     * the image and the image color values.
     * \param xPixelCoordinate x-axis coordinate of the cursor.
     * \param yPixelCoordinate y-axis coordinate of the cursor.
     */
    void displayCursorInfo(int xPixelCoordinate, int yPixelCoordinate);

    /*!
     * \brief displayImageInfo displays information about the image like
     * its size, format ...
     */
    void displayImageInfo();

    /*!
     * \brief openHistogram opens a new Histogram for the currently opened image.
     */
    void openHistogram(HistogramType type);


    /*!
     * \brief convertCoordinates of the cursor with respect to the imageLabel
     * into coordinates valid for the Image ( inside its limits ).
     * \param posX the cursor x-position.
     * \param posY the cursor y-position.
     * \return both of the coordinates with respect to the image.
     */
    QPoint convertCoordinates(float posX, float posY);

    /*!
     * \brief limitBoundaries makes sure that the cursor position cannot get
     * further than the dimension of the image.
     * \param posX position of the cursor in the x-axis.
     * \param posY position of the cursor in the y-axis.
     */
    void limitBoundaries(int &posX, int &posY);

    void openImageAdjuster();
    void openLinearTransformation();
    void openHistogramSpecification();
    void convertToGray();


private slots:
    void refreshImage(Image image);
    void refreshImage();

private:

    /*!
     * \brief mImagePixMap is a QPixmap needed to
     * represent the image in the GUI.
     */
    QPixmap mImagePixMap;
    RubberBand* mRubberBand;
    Image mImage;
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
