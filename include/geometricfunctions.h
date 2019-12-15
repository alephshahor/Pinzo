#ifndef GEOMETRICFUNCTIONS_H
#define GEOMETRICFUNCTIONS_H

#include "include/image.h"

class GeometricFunctions
{
public:
    GeometricFunctions(Image image);
    Image verticalMirror();
    Image horizontalMirror();
    Image traspose();
    Image rotation(std::string direction);

signals:
    void imageChanged(Image image);

private:
    Image image;
};

#endif // GEOMETRICFUNCTIONS_H
