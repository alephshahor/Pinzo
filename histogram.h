#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "include/image.h"
#include <QWidget>
#include <QtCharts>
namespace Ui {
class Histogram;
}

class Histogram : public QWidget
{
    Q_OBJECT

public:
    explicit Histogram(Image image, QWidget *parent = nullptr);
    ~Histogram();

private:


private:
    Ui::Histogram *ui;
    Image mImage;
};

#endif // HISTOGRAM_H
