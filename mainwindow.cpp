#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "include/image.h"
#include "imageviewer.h"

#include <iostream>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectSignals();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectSignals()
{
    connect(ui -> actionOpen, &QAction::triggered,
            this, &MainWindow::openImageViewer);
}

void MainWindow::openImageViewer(){
    QString filepath = QFileDialog::getOpenFileName((this), "Open the file");
    Image image = Image(filepath);
    ImageViewer* imageviewer = new ImageViewer();
    imageviewer -> setImage(image);
    imageviewer -> show();
}
