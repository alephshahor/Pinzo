#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "include/image.h"
#include "saveimagedialog.h"
#include "absolutehistogram.h"
#include "cumulativehistogram.h"
#include "imageadjuster.h"
#include "lineartransformation.h"
#include "histogramspecification.h"
#include "gammacorrection.h"
#include "imagedifference.h"
#include "differencemap.h"

#include <iostream>
#include <memory>
#include <QFileDialog>
#include <QDebug>
#include <QMouseEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      mImagePixMap(),
      mImage(),
      ui(new Ui::MainWindow)
{
    ui -> setupUi(this);


    // Alows mouse tracking so we can get the cursor
    // position at every moment.
    ui -> imageLabel -> setMouseTracking(true);
    ui -> centralwidget -> setMouseTracking(true);

    // We install an event filter so we can override
    // the behaviour of the widget when certain event
    // happens.
    ui -> centralwidget -> installEventFilter(this);

    statusBar() -> hide();
    connectSignals();
    scaleImageLabel();

    mRubberBand = new RubberBand(ui -> imageLabel);


}

MainWindow::~MainWindow()
{
    delete ui;
}

Image MainWindow::image()
{
    return mImage;
}

void MainWindow::setImage(const Image &image)
{
    mImage = image;
    mImagePixMap.convertFromImage(mImage.getImage());
    ui -> imageLabel -> setMargin(0);
    ui -> imageLabel -> setPixmap(mImagePixMap);
    mRubberBand = new RubberBand(ui -> imageLabel);

}

void MainWindow::refreshImage()
{
    mImagePixMap.convertFromImage(mImage.getImage());
    ui -> imageLabel -> setMargin(0);
    ui -> imageLabel -> setPixmap(mImagePixMap);
    mRubberBand = new RubberBand(ui -> imageLabel);
}

void MainWindow::refreshImage(Image image)
{
      setImage(image);
}

void MainWindow::cloneWindow()
{
    MainWindow* newWindow = new MainWindow();
    newWindow -> setImage(mImage);
    newWindow -> setAttribute(Qt::WA_DeleteOnClose);
    newWindow -> displayImageInfo();
    newWindow -> show();
}

void MainWindow::openNewWindow()
{
    MainWindow* newWindow = new MainWindow();
    newWindow -> setAttribute(Qt::WA_DeleteOnClose);
    newWindow -> show();
}

void MainWindow::openNewWindow_(Image image)
{
    MainWindow* newWindow = new MainWindow();
    newWindow -> setAttribute(Qt::WA_DeleteOnClose);
    newWindow -> openImage_(image);
    newWindow -> show();
}

void MainWindow::connectSignals()
{
    connect(ui -> actionOpen, &QAction::triggered,
            this, &MainWindow::openImage);
    connect(ui -> actionClone, &QAction::triggered,
            this, &MainWindow::cloneWindow);
    connect(ui -> actionNew, &QAction::triggered,
            this, &MainWindow::openNewWindow);
    connect(ui -> actionSave, &QAction::triggered,
            this, &MainWindow::saveImage);
    connect(ui -> actionSave_As, &QAction::triggered,
            this, &MainWindow::saveImageAs);
    connect(ui -> actionAbsoluteHist, &QAction::triggered,
            this, [this]{openHistogram(Absolute);});
    connect(ui -> actionCumulativeHist, &QAction::triggered,
            this, [this]{openHistogram(Cumulative);});
    connect(ui -> actionAdjust, &QAction::triggered,
            this, &MainWindow::openImageAdjuster);
    connect(ui -> actionLinearTransformation, &QAction::triggered,
            this, &MainWindow::openLinearTransformation);
    connect(ui -> actionToGray, &QAction::triggered,
            this, &MainWindow::convertToGray);
    connect(ui -> actionHistogram_Specification, &QAction::triggered,
            this, &MainWindow::openHistogramSpecification);
    connect(ui -> actionGamma_Correction, &QAction::triggered,
            this, &MainWindow::openGammaCorrection);
    connect(ui -> actionDifference, &QAction::triggered,
            this, &MainWindow::openImageDifference);
    connect(ui -> actionDifference_Map, &QAction::triggered,
            this, &MainWindow::openDifferenceMap);
}

void MainWindow::openImage(){
    QString filepath = QFileDialog::getOpenFileName((this), "Open the file");
    Image image(filepath);
    setImage(image);
    setWindowTitle(image.getImageName());
    displayImageInfo();
}

void MainWindow::openImage_(Image image){
    setImage(image);
    setWindowTitle(image.getImageName());
    displayImageInfo();
}

void MainWindow::saveImage()
{

    mImage.getImage().save(mImage.getImagePath(), mImage.getImageFormat().toStdString().c_str() , 100);

}

void MainWindow::saveImageAs()
{

    // Gets the directory path where the user wants to store the image.
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select target directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    // Opens a Dialog to retrieve the name, format and quality of the image to be saved.
    SaveImageDialog* imageDialog = new SaveImageDialog(this);

    imageDialog -> exec();
    QString fileName   = imageDialog -> getFileName();
    QString fileFormat = imageDialog -> getFileFormat();
    int fileQuality = imageDialog -> getFileQuality();


    QString newPath(dir + "/" + fileName);
    mImage.getImage().save(newPath, fileFormat.toStdString().c_str() , fileQuality);

    delete imageDialog;

}

void MainWindow::scaleImageLabel()
{
    ui -> imageLabel -> setScaledContents( true );
    ui -> imageLabel -> setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (dynamic_cast<QWidget*>(obj) == ui -> centralwidget){

        QMouseEvent* event_ = static_cast<QMouseEvent*>(event);

        int offset = ui -> imageInfoFrameTop -> geometry().height();
        float posX = event_ -> pos().x();
        float posY = event_ -> pos().y() - offset;

        QPoint imgCoordinates = convertCoordinates(posX, posY);


        if(event -> type() == QMouseEvent::MouseMove){

            mRubberBand -> mouseMoveEvent(QPoint(posX,posY));
            displayCursorInfo(imgCoordinates.x(), imgCoordinates.y());

        }else if (event -> type() == QMouseEvent::MouseButtonPress){

            mRubberBand -> mousePressEvent(QPoint(posX,posY), imgCoordinates);

        } else if(event -> type() == QMouseEvent::MouseButtonRelease){

            try {
                QRect selection =  mRubberBand -> mouseReleaseEvent(QPoint(posX,posY), imgCoordinates);
                Image croppedImage(mImage, selection);
                openNewWindow_(croppedImage);
            } catch (const char* msg) {
                QMessageBox errorMessageBox;
                errorMessageBox.critical(0, "Error", msg);
                errorMessageBox.setFixedSize(500,200);
            }


        }return false;

    }else return QMainWindow::eventFilter(obj, event);
   }

QPoint MainWindow::convertCoordinates(float posX, float posY)
{
    int labelWidth = ui -> imageLabel -> width();
    int labelHeight = ui -> imageLabel -> height();

    posX = posX / labelWidth;
    posY = posY / labelHeight;

    int imageWidth = mImage.getImage().width();
    int imageHeight = mImage.getImage().height();

    int imgPosX = posX * imageWidth;
    int imgPosY = posY * imageHeight;

    limitBoundaries(imgPosX, imgPosY);

    return QPoint(imgPosX, imgPosY);
}

void MainWindow::limitBoundaries(int& imgPosX, int& imgPosY){

    int imageWidth = mImage.getImage().width();
    int imageHeight = mImage.getImage().height();

    if(imgPosY < 0){
        imgPosY = 0;
    }

    if(imgPosX < 0){
        imgPosX = 0;
    }

    if(imgPosX > imageWidth){
        imgPosX = imageWidth;
    }

    if(imgPosY > imageHeight){
        imgPosY = imageHeight;
    }
}

void MainWindow::openImageAdjuster()
{
    ImageAdjuster* imgAdjuster = new ImageAdjuster(mImage, nullptr);
    connect(imgAdjuster, SIGNAL(imageChanged(Image)), this, SLOT(refreshImage(Image)));
    imgAdjuster  -> setAttribute(Qt::WA_DeleteOnClose);
    imgAdjuster  -> show();
}

void MainWindow::openLinearTransformation()
{
    LinearTransformation* linearTransformation = new LinearTransformation(mImage);
    connect(linearTransformation, SIGNAL(imageChanged()), this, SLOT(refreshImage()));
    linearTransformation -> setAttribute(Qt::WA_DeleteOnClose);
    linearTransformation -> show();
}

void MainWindow::openHistogramSpecification()
{
    HistogramSpecification* histogramSpecification = new HistogramSpecification(mImage);
    connect(histogramSpecification, SIGNAL(imageChanged()), this, SLOT(refreshImage()));
    histogramSpecification -> setAttribute(Qt::WA_DeleteOnClose);
    histogramSpecification  -> show();
}

void MainWindow::convertToGray(){
    ImageAdjuster::toGrayscale(mImage);
    refreshImage();
}

void MainWindow::openGammaCorrection()
{
    GammaCorrection* gammaCorrection = new GammaCorrection(mImage, nullptr);
    connect(gammaCorrection, SIGNAL(imageChanged(Image)), this, SLOT(refreshImage(Image)));
    gammaCorrection -> setAttribute(Qt::WA_DeleteOnClose);
    gammaCorrection -> show();
}

void MainWindow::openImageDifference()
{
    ImageDifference* imageDifference = new ImageDifference(mImage, nullptr);
    connect(imageDifference, SIGNAL(imageChanged(Image)), this, SLOT(refreshImage(Image)));
    imageDifference -> setAttribute(Qt::WA_DeleteOnClose);
    imageDifference -> show();
}

void MainWindow::openDifferenceMap()
{
    DifferenceMap* differenceMap = new DifferenceMap(mImage, nullptr);
    connect(differenceMap, SIGNAL(imageChanged(Image)), this, SLOT(refreshImage(Image)));
    differenceMap -> setAttribute(Qt::WA_DeleteOnClose);
    differenceMap -> show();
}

void MainWindow::displayCursorInfo(int xPixelCoordinate, int yPixelCoordinate){

    QColor imagePixel = mImage.getImage().pixel(xPixelCoordinate, yPixelCoordinate);
    int red = imagePixel.red();
    int green = imagePixel.green();
    int blue = imagePixel.blue();

    QString positionText;
    positionText = QString("Position: (%1 , %2)").arg(
                    xPixelCoordinate).arg(yPixelCoordinate);

    QString valueText;
    valueText = QString("Value: (%1 , %2 , %3)").arg(
                    red).arg(green).arg(blue);

    ui -> positionLabel  -> setText(positionText);
    ui -> valueLabel -> setText(valueText);
}

void MainWindow::displayImageInfo()
{
    QString formatLabelText = "Format: " + mImage.getImageFormat();
    ui -> formatLabel -> setText(formatLabelText);

    QString dimensionLabelText = "Dimension: " + QString::number(mImage.getImageDimension().first) + " x "
                                               + QString::number(mImage.getImageDimension().second);
    ui -> dimensionLabel -> setText(dimensionLabelText);

    QString depthLabelText = "Depth: " + QString::number(mImage.getImageDepth());

    QString colorLabelText = "Color: ";
    if(mImage.isGray() == true){
        colorLabelText += "Grayscale";
    }else colorLabelText += "RGB";

    ui -> colorLabel -> setText(colorLabelText);

    ui -> depthLabel -> setText(depthLabelText);
}

void MainWindow::openHistogram(HistogramType type)
{
    Histogram* h;

    if(type == Cumulative){
        h = new CumulativeHistogram(mImage, nullptr);
    }else{
        h = new AbsoluteHistogram(mImage, nullptr);
    }

    h -> setAttribute(Qt::WA_DeleteOnClose);
    h -> displayHistogram();
    h -> show();
}


