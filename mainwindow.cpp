#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "include/image.h"
#include "saveimagedialog.h"
#include "histogram.h"

#include <iostream>
#include <memory>
#include <QFileDialog>
#include <QDebug>
#include <QMouseEvent>

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

Image MainWindow::image() const
{
    return mImage;
}

void MainWindow::setImage(const Image &image)
{
    mImage = image;
    mImagePixMap.convertFromImage(mImage.image());
    ui -> imageLabel -> setMargin(0);
    ui -> imageLabel -> setPixmap(mImagePixMap);
    mRubberBand = new RubberBand(ui -> imageLabel);

}

void MainWindow::cloneWindow()
{
    MainWindow* newWindow = new MainWindow();
    newWindow -> setImage(mImage);
    newWindow -> setAttribute(Qt::WA_DeleteOnClose);
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
    connect(ui -> actionHistogram, &QAction::triggered,
            this, &MainWindow::openHistogram);
}

void MainWindow::openImage(){
    QString filepath = QFileDialog::getOpenFileName((this), "Open the file");
    Image image = Image(filepath);
    setImage(image);
    setWindowTitle(image.fileName());
    displayImageInfo();
}

void MainWindow::openImage_(Image image){
    setImage(image);
    setWindowTitle(image.fileName());
    displayImageInfo();
}

void MainWindow::saveImage()
{

    mImage.image().save(mImage.filePath(), mImage.fileFormat().toStdString().c_str() , 100);

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
    mImage.image().save(newPath, fileFormat.toStdString().c_str() , fileQuality);

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

    int imageWidth = mImage.image().width();
    int imageHeight = mImage.image().height();

    int imgPosX = posX * imageWidth;
    int imgPosY = posY * imageHeight;

    limitBoundaries(imgPosX, imgPosY);

    return QPoint(imgPosX, imgPosY);
}

void MainWindow::limitBoundaries(int& imgPosX, int& imgPosY){

    int imageWidth = mImage.image().width();
    int imageHeight = mImage.image().height();

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

void MainWindow::displayCursorInfo(int xPixelCoordinate, int yPixelCoordinate){

    QColor imagePixel = mImage.image().pixel(xPixelCoordinate, yPixelCoordinate);
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
    QString formatLabelText = "Format: " + mImage.fileFormat();
    ui -> formatLabel -> setText(formatLabelText);

    QString dimensionLabelText = "Dimension: " + QString::number(mImage.fileDimension().first) + " x "
                                               + QString::number(mImage.fileDimension().second);
    ui -> dimensionLabel -> setText(dimensionLabelText);

    QString depthLabelText = "Depth: " + QString::number(mImage.fileDepth());

    QString colorLabelText = "Color: ";
    if(mImage.isGray() == true){
        colorLabelText += "Grayscale";
    }else colorLabelText += "RGB";

    ui -> colorLabel -> setText(colorLabelText);

    ui -> depthLabel -> setText(depthLabelText);
}

void MainWindow::openHistogram()
{
    Histogram* h = new Histogram(mImage);
    h -> setAttribute(Qt::WA_DeleteOnClose);
    h -> show();
}


