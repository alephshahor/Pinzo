#ifndef CUMULATIVEHISTOGRAM_H
#define CUMULATIVEHISTOGRAM_H

#include <QWidget>

namespace Ui {
class CumulativeHistogram;
}

class CumulativeHistogram : public QWidget
{
    Q_OBJECT

public:
    explicit CumulativeHistogram(QWidget *parent = nullptr);
    ~CumulativeHistogram();

private:
    Ui::CumulativeHistogram *ui;
};

#endif // CUMULATIVEHISTOGRAM_H
