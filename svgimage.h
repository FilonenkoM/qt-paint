#ifndef SVGIMAGE_H
#define SVGIMAGE_H

#include <QColor>
#include <QSize>
#include "imagefragment.h"
#include <vector>

using namespace std;

class SVGImage
{
    QSize _size;
    vector<ImageFragment> _fragments;

public:
    SVGImage();
    inline QSize& size() { return _size; }
    inline vector<ImageFragment>& fragments() { return _fragments; }
    void paint(QPainter& painter);

    ImageFragment* findClosestFragment(QPoint point);

};


#endif // SVGIMAGE_H
