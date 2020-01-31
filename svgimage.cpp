#include "svgimage.h"
#include <QPainter>
#include <QPointF>
#include <QDebug>

SVGImage::SVGImage()
{
}

void SVGImage::paint(QPainter &painter)
{
    for(ImageFragment fragment : _fragments)
    {
        painter.setPen(fragment.pen());
        painter.setBrush(fragment.brush());

//        painter.save();



        painter.drawPath(fragment.path());

//        painter.restore();
    }
}

ImageFragment* SVGImage::findClosestFragment(QPoint point) {
    ImageFragment* fragment = nullptr;;

    for(ImageFragment& fragment : _fragments) {
        if(fragment.path().boundingRect().contains(point)) {
            return &fragment;
        }
    }

    return fragment;
}
