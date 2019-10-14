#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "include/image.h"

#include <iostream>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      mImagePixMap(),
      mImage(),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectSignals();
    scaleImage();
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
    ui -> imageLabel -> setPixmap(mImagePixMap);
}

void MainWindow::clone()
{
    MainWindow* newWindow = new MainWindow();
    newWindow -> setImage(mImage);
    newWindow -> show();
}

void MainWindow::connectSignals()
{
    connect(ui -> actionOpen, &QAction::triggered,
            this, &MainWindow::openImageViewer);
    connect(ui -> actionClone, &QAction::triggered,
            this, &MainWindow::clone);
}

void MainWindow::openImageViewer(){
    QString filepath = QFileDialog::getOpenFileName((this), "Open the file");
    Image image = Image(filepath);
    setImage(image);
}

void MainWindow::scaleImage()
{
    ui -> imageLabel -> setScaledContents( true );
    ui -> imageLabel -> setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
}
