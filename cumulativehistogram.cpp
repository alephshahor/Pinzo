#include "cumulativehistogram.h"
#include "ui_cumulativehistogram.h"

CumulativeHistogram::CumulativeHistogram(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CumulativeHistogram)
{
    ui->setupUi(this);
}

CumulativeHistogram::~CumulativeHistogram()
{
    delete ui;
}
