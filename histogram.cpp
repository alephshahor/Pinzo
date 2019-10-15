#include "histogram.h"
#include "ui_histogram.h"

#include <QBarSet>
#include <vector>
#include <QChartView>
#include <iostream>

Histogram::Histogram(Image image, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Histogram),
    mImage(image)
{
    ui->setupUi(this);
}

Histogram::~Histogram()
{
    delete ui;
}

void Histogram::createHistogram()
{
}
