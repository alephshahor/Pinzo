#ifndef ROTATION_H
#define ROTATION_H

#include <QWidget>
#include "include/image.h"

namespace Ui {
class Rotation;
}

class Rotation : public QWidget
{
    Q_OBJECT

public:
    explicit Rotation(Image image, QWidget *parent = nullptr);
    ~Rotation();

    void rotateImage();
    void directTransform(float rotationAngle);
    void inverseTransform(float rotationAngle);
    std::vector<std::vector<float>> generateRotationMatrix(float rotationAngle);

    int maxIndex(std::vector<int> indexes);
    int minIndex(std::vector<int> indexes);

    int calculateParallelogramArea(int leftmostUpperCorner[], int rightmostUpperCorner[],
                                  int leftmostLowerCorner[], int rightmostLowerCorner[]);
    bool pointIsInsideImage(int leftmostUpperCorner[], int rightmostUpperCorner[],
                            int leftmostLowerCorner[], int rightmostLowerCorner[],
                            int rotatedPoint[]);
    int calculateTriangleArea(int a[], int b[], int c[]);

signals:
    void imageChanged(Image image);

private:
    Ui::Rotation *ui;
    Image image;
    enum rotationTransformations {directTransformationRotation, inverseTransformationRotation};
};

#endif // ROTATION_H
