#include "lineartransformation.h"
#include "ui_lineartransformation.h"
#include <QtAlgorithms>
#include <QMessageBox>

LinearTransformation::LinearTransformation(Image& image,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LinearTransformation),
    mImage(image)
{
    ui->setupUi(this);

    mTransformedSpaceValues = QVector<double>(pow(2,mImage.getImageDepth()));
    mTransformedSpaceKeys = QVector<double>(pow(2,mImage.getImageDepth()));
    calculateSpaceKeys();

    connect(ui -> addButton, &QPushButton::clicked,
            this, &LinearTransformation::addPoint);
    connect(ui -> removeButton, &QPushButton::clicked,
            this, &LinearTransformation::removePoint);
    connect(ui -> transformButton, &QPushButton::clicked,
            this, &LinearTransformation::transformImage);
}

LinearTransformation::~LinearTransformation()
{
    delete ui;
}

void LinearTransformation::transformImage(){

    QRgb *st = (QRgb *) mImage.getImage().bits();
    quint64 pixelCount = mImage.getWidth() * mImage.getHeight();

    for (quint64 p = 0; p < pixelCount; p++) {

        st[p] = QColor( mTransformedSpaceValues[qRed(st[p])],
                        mTransformedSpaceValues[qGreen(st[p])],
                        mTransformedSpaceValues[qBlue(st[p])]
                       ).rgb();
    }
    emit imageChanged();
}

void LinearTransformation::addPoint()
{
    int xPoint = ui -> xInput->text().toInt();
    int yPoint = ui -> yInput->text().toInt();

    int pointIndex = exists(xPoint);
    if(pointIndex != -1){
        mPoints[pointIndex] = QPoint(xPoint, yPoint);
    }else mPoints.append(QPoint(xPoint, yPoint));
    calculateTransformedSpace();
    constructPlot();
}

void LinearTransformation::removePoint()
{
    int xPoint = ui -> xInput->text().toInt();
    int yPoint = ui -> yInput->text().toInt();

    int pointIndex = exists(xPoint);
    if(pointIndex != -1){
        mPoints.remove(pointIndex);
        calculateTransformedSpace();
        constructPlot();
    }else {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "Point doesn't exists");
        messageBox.setFixedSize(500,200);
    }
}

void LinearTransformation::calculateTransformedSpace()
{
    if(mPoints.empty()){
        throw "No points declared\n";
    }

    sortPoints();


    for(int i = 0; i < mPoints.size() - 1; i++){
        double slope = calculateSlope(mPoints[i], mPoints[i+1]);
        Line line = {slope, mPoints[i].y(), mPoints[i].x()};
        applyTransformation(line, mPoints[i].x(), mPoints[i+1].x());
    }

}

void LinearTransformation::applyTransformation(LinearTransformation::Line line, int minRange, int maxRange)
{
     for(int i = minRange; i <= maxRange; i++){
         mTransformedSpaceValues[i] = line.calculateValue(i);
     }
}

double LinearTransformation::calculateSlope(QPoint p1, QPoint p2)
{
    return static_cast<double>(p2.y() - p1.y()) / static_cast<double>(p2.x() - p1.x());
}

int LinearTransformation::exists(int x)
{
    for(int i = 0; i < mPoints.size(); i++){
        if(mPoints[i].x() == x)
            return i;
    }
    return -1;
}

void LinearTransformation::sortPoints()
{
    quicksort(0, mPoints.size()-1);
}

void LinearTransformation::quicksort(int lo, int hi)
{
    if(lo < hi){
        int p = partition(lo, hi);
        quicksort(lo, p - 1);
        quicksort(p + 1, hi);
    }
}

void LinearTransformation::swap(int i, int j){
    QPoint temp = mPoints[i];
    mPoints[i] = mPoints[j];
    mPoints[j] = temp;
}

int LinearTransformation::partition(int lo, int hi)
{
    QPoint pivot = mPoints[hi];
    int i = lo;
    for(int j = lo; j < hi;  j++){
        if(mPoints[j].x() < pivot.x()){
            swap(i,j);
            i += 1;
        }
    }
    swap(i, hi);
    return i;
}

void LinearTransformation::constructPlot()
{
    ui -> transformationPlot -> addGraph();
    QColor color(20+200/4.0,70*(1.6/4.0), 150, 150);
    ui -> transformationPlot -> graph( )-> setLineStyle(QCPGraph::lsLine);
    ui -> transformationPlot -> graph() -> setPen(QPen(color.lighter(200)));
    ui -> transformationPlot -> graph() -> setBrush(QBrush(color));
    ui -> transformationPlot -> graph(0)-> setData(mTransformedSpaceKeys,mTransformedSpaceValues);
    ui -> transformationPlot -> xAxis -> setRange(0, pow(2,mImage.getImageDepth()));
    ui -> transformationPlot -> yAxis -> setRange(0, pow(2,mImage.getImageDepth()));
    ui -> transformationPlot -> replot();
}


void LinearTransformation::calculateSpaceKeys()
{
    for(int i = 0; i < pow(2,mImage.getImageDepth()); i++){
        mTransformedSpaceKeys[i] = static_cast<double>(i);
    }
}


